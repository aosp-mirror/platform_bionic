/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "linker_config.h"

#include "linker_globals.h"
#include "linker_debug.h"
#include "linker_utils.h"

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <android-base/strings.h>

#include <async_safe/log.h>

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <unordered_map>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

class ConfigParser {
 public:
  enum {
    kPropertyAssign,
    kPropertyAppend,
    kSection,
    kEndOfFile,
    kError,
  };

  explicit ConfigParser(std::string&& content)
      : content_(std::move(content)), p_(0), lineno_(0), was_end_of_file_(false) {}

  /*
   * Possible return values
   * kPropertyAssign: name is set to property name and value is set to property value
   * kPropertyAppend: same as kPropertyAssign, but the value should be appended
   * kSection: name is set to section name.
   * kEndOfFile: reached end of file.
   * kError: error_msg is set.
   */
  int next_token(std::string* name, std::string* value, std::string* error_msg) {
    std::string line;
    while(NextLine(&line)) {
      size_t found = line.find('#');
      line = android::base::Trim(line.substr(0, found));

      if (line.empty()) {
        continue;
      }

      if (line[0] == '[' && line.back() == ']') {
        *name = line.substr(1, line.size() - 2);
        return kSection;
      }

      size_t found_assign = line.find('=');
      size_t found_append = line.find("+=");
      if (found_assign != std::string::npos && found_append == std::string::npos) {
        *name = android::base::Trim(line.substr(0, found_assign));
        *value = android::base::Trim(line.substr(found_assign + 1));
        return kPropertyAssign;
      }

      if (found_append != std::string::npos) {
        *name = android::base::Trim(line.substr(0, found_append));
        *value = android::base::Trim(line.substr(found_append + 2));
        return kPropertyAppend;
      }

      *error_msg = std::string("invalid format: ") +
                   line +
                   ", expected \"name = property\", \"name += property\", or \"[section]\"";
      return kError;
    }

    // to avoid infinite cycles when programmer makes a mistake
    CHECK(!was_end_of_file_);
    was_end_of_file_ = true;
    return kEndOfFile;
  }

  size_t lineno() const {
    return lineno_;
  }

 private:
  bool NextLine(std::string* line) {
    if (p_ == std::string::npos) {
      return false;
    }

    size_t found = content_.find('\n', p_);
    if (found != std::string::npos) {
      *line = content_.substr(p_, found - p_);
      p_ = found + 1;
    } else {
      *line = content_.substr(p_);
      p_ = std::string::npos;
    }

    lineno_++;
    return true;
  }

  std::string content_;
  size_t p_;
  size_t lineno_;
  bool was_end_of_file_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ConfigParser);
};

class PropertyValue {
 public:
  PropertyValue() = default;

  PropertyValue(std::string&& value, size_t lineno)
    : value_(std::move(value)), lineno_(lineno) {}

  const std::string& value() const {
    return value_;
  }

  void append_value(std::string&& value) {
    value_ = value_ + value;
    // lineno isn't updated as we might have cases like this:
    // property.x = blah
    // property.y = blah
    // property.x += blah
  }

  size_t lineno() const {
    return lineno_;
  }

 private:
  std::string value_;
  size_t lineno_;
};

static std::string create_error_msg(const char* file,
                                    size_t lineno,
                                    const std::string& msg) {
  char buf[1024];
  async_safe_format_buffer(buf, sizeof(buf), "%s:%zu: error: %s", file, lineno, msg.c_str());

  return std::string(buf);
}

static bool parse_config_file(const char* ld_config_file_path,
                              const char* binary_realpath,
                              std::unordered_map<std::string, PropertyValue>* properties,
                              std::string* error_msg) {
  std::string content;
  if (!android::base::ReadFileToString(ld_config_file_path, &content)) {
    if (errno != ENOENT) {
      *error_msg = std::string("error reading file \"") +
                   ld_config_file_path + "\": " + strerror(errno);
    }
    return false;
  }

  ConfigParser cp(std::move(content));

  std::string section_name;

  while (true) {
    std::string name;
    std::string value;
    std::string error;

    int result = cp.next_token(&name, &value, &error);
    if (result == ConfigParser::kError) {
      DL_WARN("%s:%zd: warning: couldn't parse %s (ignoring this line)",
              ld_config_file_path,
              cp.lineno(),
              error.c_str());
      continue;
    }

    if (result == ConfigParser::kSection || result == ConfigParser::kEndOfFile) {
      return false;
    }

    if (result == ConfigParser::kPropertyAssign) {
      if (!android::base::StartsWith(name, "dir.")) {
        DL_WARN("%s:%zd: warning: unexpected property name \"%s\", "
                "expected format dir.<section_name> (ignoring this line)",
                ld_config_file_path,
                cp.lineno(),
                name.c_str());
        continue;
      }

      // remove trailing '/'
      while (!value.empty() && value.back() == '/') {
        value.pop_back();
      }

      if (value.empty()) {
        DL_WARN("%s:%zd: warning: property value is empty (ignoring this line)",
                ld_config_file_path,
                cp.lineno());
        continue;
      }

      // If the path can be resolved, resolve it
      char buf[PATH_MAX];
      std::string resolved_path;
      if (access(value.c_str(), R_OK) != 0) {
        if (errno == ENOENT) {
          // no need to test for non-existing path. skip.
          continue;
        }
        // If not accessible, don't call realpath as it will just cause
        // SELinux denial spam. Use the path unresolved.
        resolved_path = value;
      } else if (realpath(value.c_str(), buf)) {
        resolved_path = buf;
      } else {
        // realpath is expected to fail with EPERM in some situations, so log
        // the failure with INFO rather than DL_WARN. e.g. A binary in
        // /data/local/tmp may attempt to stat /postinstall. See
        // http://b/120996057.
        INFO("%s:%zd: warning: path \"%s\" couldn't be resolved: %s",
             ld_config_file_path,
             cp.lineno(),
             value.c_str(),
             strerror(errno));
        resolved_path = value;
      }

      if (file_is_under_dir(binary_realpath, resolved_path)) {
        section_name = name.substr(4);
        break;
      }
    }
  }

  INFO("[ Using config section \"%s\" ]", section_name.c_str());

  // skip everything until we meet a correct section
  while (true) {
    std::string name;
    std::string value;
    std::string error;

    int result = cp.next_token(&name, &value, &error);

    if (result == ConfigParser::kSection && name == section_name) {
      break;
    }

    if (result == ConfigParser::kEndOfFile) {
      *error_msg = create_error_msg(ld_config_file_path,
                                    cp.lineno(),
                                    std::string("section \"") + section_name + "\" not found");
      return false;
    }
  }

  // found the section - parse it
  while (true) {
    std::string name;
    std::string value;
    std::string error;

    int result = cp.next_token(&name, &value, &error);

    if (result == ConfigParser::kEndOfFile || result == ConfigParser::kSection) {
      break;
    }

    if (result == ConfigParser::kPropertyAssign) {
      if (properties->contains(name)) {
        DL_WARN("%s:%zd: warning: redefining property \"%s\" (overriding previous value)",
                ld_config_file_path,
                cp.lineno(),
                name.c_str());
      }

      (*properties)[name] = PropertyValue(std::move(value), cp.lineno());
    } else if (result == ConfigParser::kPropertyAppend) {
      if (!properties->contains(name)) {
        DL_WARN("%s:%zd: warning: appending to undefined property \"%s\" (treating as assignment)",
                ld_config_file_path,
                cp.lineno(),
                name.c_str());
        (*properties)[name] = PropertyValue(std::move(value), cp.lineno());
      } else {
        if (android::base::EndsWith(name, ".links") ||
            android::base::EndsWith(name, ".namespaces")) {
          value = "," + value;
          (*properties)[name].append_value(std::move(value));
        } else if (android::base::EndsWith(name, ".paths") ||
                   android::base::EndsWith(name, ".shared_libs") ||
                   android::base::EndsWith(name, ".whitelisted") ||
                   android::base::EndsWith(name, ".allowed_libs")) {
          value = ":" + value;
          (*properties)[name].append_value(std::move(value));
        } else {
          DL_WARN("%s:%zd: warning: += isn't allowed for property \"%s\" (ignoring)",
                  ld_config_file_path,
                  cp.lineno(),
                  name.c_str());
        }
      }
    }

    if (result == ConfigParser::kError) {
      DL_WARN("%s:%zd: warning: couldn't parse %s (ignoring this line)",
              ld_config_file_path,
              cp.lineno(),
              error.c_str());
      continue;
    }
  }

  return true;
}

static Config g_config;

static constexpr const char* kDefaultConfigName = "default";
static constexpr const char* kPropertyAdditionalNamespaces = "additional.namespaces";

class Properties {
 public:
  explicit Properties(std::unordered_map<std::string, PropertyValue>&& properties)
      : properties_(std::move(properties)), target_sdk_version_(__ANDROID_API__) {}

  std::vector<std::string> get_strings(const std::string& name, size_t* lineno = nullptr) const {
    auto it = find_property(name, lineno);
    if (it == properties_.end()) {
      // return empty vector
      return std::vector<std::string>();
    }

    std::vector<std::string> strings = android::base::Split(it->second.value(), ",");

    for (size_t i = 0; i < strings.size(); ++i) {
      strings[i] = android::base::Trim(strings[i]);
    }

    return strings;
  }

  bool get_bool(const std::string& name, size_t* lineno = nullptr) const {
    auto it = find_property(name, lineno);
    if (it == properties_.end()) {
      return false;
    }

    return it->second.value() == "true";
  }

  std::string get_string(const std::string& name, size_t* lineno = nullptr) const {
    auto it = find_property(name, lineno);
    return (it == properties_.end()) ? "" : it->second.value();
  }

  std::vector<std::string> get_paths(const std::string& name, bool resolve, size_t* lineno = nullptr) {
    std::string paths_str = get_string(name, lineno);

    std::vector<std::string> paths;
    split_path(paths_str.c_str(), ":", &paths);

    std::vector<std::pair<std::string, std::string>> params;
    params.push_back({ "LIB", kLibPath });
    if (target_sdk_version_ != 0) {
      char buf[16];
      async_safe_format_buffer(buf, sizeof(buf), "%d", target_sdk_version_);
      params.push_back({ "SDK_VER", buf });
    }

    static std::string vndk_ver = Config::get_vndk_version_string('-');
    params.push_back({ "VNDK_VER", vndk_ver });
    static std::string vndk_apex_ver = Config::get_vndk_version_string('v');
    params.push_back({ "VNDK_APEX_VER", vndk_apex_ver });

    for (auto& path : paths) {
      format_string(&path, params);
    }

    if (resolve) {
      std::vector<std::string> resolved_paths;
      for (const auto& path : paths) {
        if (path.empty()) {
          continue;
        }
        // this is single threaded. no need to lock
        auto cached = resolved_paths_.find(path);
        if (cached == resolved_paths_.end()) {
          resolved_paths_[path] = resolve_path(path);
          cached = resolved_paths_.find(path);
        }
        CHECK(cached != resolved_paths_.end());
        if (cached->second.empty()) {
          continue;
        }
        resolved_paths.push_back(cached->second);
      }

      return resolved_paths;
    } else {
      return paths;
    }
  }

  void set_target_sdk_version(int target_sdk_version) {
    target_sdk_version_ = target_sdk_version;
  }

 private:
  std::unordered_map<std::string, PropertyValue>::const_iterator
  find_property(const std::string& name, size_t* lineno) const {
    auto it = properties_.find(name);
    if (it != properties_.end() && lineno != nullptr) {
      *lineno = it->second.lineno();
    }

    return it;
  }
  std::unordered_map<std::string, PropertyValue> properties_;
  std::unordered_map<std::string, std::string> resolved_paths_;
  int target_sdk_version_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Properties);
};

bool Config::read_binary_config(const char* ld_config_file_path,
                                      const char* binary_realpath,
                                      bool is_asan,
                                      bool is_hwasan,
                                      const Config** config,
                                      std::string* error_msg) {
  g_config.clear();

  std::unordered_map<std::string, PropertyValue> property_map;
  if (!parse_config_file(ld_config_file_path, binary_realpath, &property_map, error_msg)) {
    return false;
  }

  Properties properties(std::move(property_map));

  auto failure_guard = android::base::make_scope_guard([] { g_config.clear(); });

  std::unordered_map<std::string, NamespaceConfig*> namespace_configs;

  namespace_configs[kDefaultConfigName] = g_config.create_namespace_config(kDefaultConfigName);

  std::vector<std::string> additional_namespaces = properties.get_strings(kPropertyAdditionalNamespaces);
  for (const auto& name : additional_namespaces) {
    namespace_configs[name] = g_config.create_namespace_config(name);
  }

  bool versioning_enabled = properties.get_bool("enable.target.sdk.version");
  int target_sdk_version = __ANDROID_API__;
  if (versioning_enabled) {
    std::string version_file = dirname(binary_realpath) + "/.version";
    std::string content;
    if (!android::base::ReadFileToString(version_file, &content)) {
      if (errno != ENOENT) {
        *error_msg = std::string("error reading version file \"") +
                     version_file + "\": " + strerror(errno);
        return false;
      }
    } else {
      content = android::base::Trim(content);
      errno = 0;
      char* end = nullptr;
      const char* content_str = content.c_str();
      int result = strtol(content_str, &end, 10);
      if (errno == 0 && *end == '\0' && result > 0) {
        target_sdk_version = result;
        properties.set_target_sdk_version(target_sdk_version);
      } else {
        *error_msg = std::string("invalid version \"") + version_file + "\": \"" + content +"\"";
        return false;
      }
    }
  }

  g_config.set_target_sdk_version(target_sdk_version);

  for (const auto& ns_config_it : namespace_configs) {
    auto& name = ns_config_it.first;
    NamespaceConfig* ns_config = ns_config_it.second;

    std::string property_name_prefix = std::string("namespace.") + name;

    size_t lineno = 0;
    std::vector<std::string> linked_namespaces =
        properties.get_strings(property_name_prefix + ".links", &lineno);

    for (const auto& linked_ns_name : linked_namespaces) {
      if (!namespace_configs.contains(linked_ns_name)) {
        *error_msg = create_error_msg(ld_config_file_path,
                                      lineno,
                                      std::string("undefined namespace: ") + linked_ns_name);
        return false;
      }

      bool allow_all_shared_libs = properties.get_bool(property_name_prefix + ".link." +
                                                       linked_ns_name + ".allow_all_shared_libs");

      std::string shared_libs = properties.get_string(property_name_prefix +
                                                      ".link." +
                                                      linked_ns_name +
                                                      ".shared_libs", &lineno);

      if (!allow_all_shared_libs && shared_libs.empty()) {
        *error_msg = create_error_msg(ld_config_file_path,
                                      lineno,
                                      std::string("list of shared_libs for ") +
                                      name +
                                      "->" +
                                      linked_ns_name +
                                      " link is not specified or is empty.");
        return false;
      }

      if (allow_all_shared_libs && !shared_libs.empty()) {
        *error_msg = create_error_msg(ld_config_file_path, lineno,
                                      std::string("both shared_libs and allow_all_shared_libs "
                                                  "are set for ") +
                                      name + "->" + linked_ns_name + " link.");
        return false;
      }

      ns_config->add_namespace_link(linked_ns_name, shared_libs, allow_all_shared_libs);
    }

    ns_config->set_isolated(properties.get_bool(property_name_prefix + ".isolated"));
    ns_config->set_visible(properties.get_bool(property_name_prefix + ".visible"));

    std::string allowed_libs =
        properties.get_string(property_name_prefix + ".whitelisted", &lineno);
    const std::string libs = properties.get_string(property_name_prefix + ".allowed_libs", &lineno);
    if (!allowed_libs.empty() && !libs.empty()) {
      allowed_libs += ":";
    }
    allowed_libs += libs;
    if (!allowed_libs.empty()) {
      ns_config->set_allowed_libs(android::base::Split(allowed_libs, ":"));
    }

    // these are affected by is_asan flag
    if (is_asan) {
      property_name_prefix += ".asan";
    } else if (is_hwasan) {
      property_name_prefix += ".hwasan";
    }

    // search paths are resolved (canonicalized). This is required mainly for
    // the case when /vendor is a symlink to /system/vendor, which is true for
    // non Treble-ized legacy devices.
    ns_config->set_search_paths(properties.get_paths(property_name_prefix + ".search.paths", true));

    // However, for permitted paths, we are not required to resolve the paths
    // since they are only set for isolated namespaces, which implies the device
    // is Treble-ized (= /vendor is not a symlink to /system/vendor).
    // In fact, the resolving is causing an unexpected side effect of selinux
    // denials on some executables which are not allowed to access some of the
    // permitted paths.
    ns_config->set_permitted_paths(properties.get_paths(property_name_prefix + ".permitted.paths", false));
  }

  failure_guard.Disable();
  *config = &g_config;
  return true;
}

std::string Config::get_vndk_version_string(const char delimiter) {
  std::string version = android::base::GetProperty("ro.vndk.version", "");
  if (version != "" && version != "current") {
    //add the delimiter char in front of the string and return it.
    return version.insert(0, 1, delimiter);
  }
  return "";
}

NamespaceConfig* Config::create_namespace_config(const std::string& name) {
  namespace_configs_.push_back(std::unique_ptr<NamespaceConfig>(new NamespaceConfig(name)));
  NamespaceConfig* ns_config_ptr = namespace_configs_.back().get();
  namespace_configs_map_[name] = ns_config_ptr;
  return ns_config_ptr;
}

void Config::clear() {
  namespace_configs_.clear();
  namespace_configs_map_.clear();
}
