/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <err.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <sys/resource.h>

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <benchmark/benchmark.h>
#include <tinyxml2.h>
#include "util.h"

#define _STR(x) #x
#define STRINGFY(x) _STR(x)

static const std::vector<int> kCommonSizes{
  8,
  64,
  512,
  1 * KB,
  8 * KB,
  16 * KB,
  32 * KB,
  64 * KB,
  128 * KB,
};

static const std::vector<int> kSmallSizes{
  // Increment by 1
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
  // Increment by 8
  24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144,
  // Increment by 16
  160, 176, 192, 208, 224, 240, 256,
};

static const std::vector<int> kMediumSizes{
  512,
  1 * KB,
  8 * KB,
  16 * KB,
  32 * KB,
  64 * KB,
  128 * KB,
};

static const std::vector<int> kLargeSizes{
  256 * KB,
  512 * KB,
  1024 * KB,
  2048 * KB,
};

static std::map<std::string, const std::vector<int> &> kSizes{
  { "SMALL",  kSmallSizes },
  { "MEDIUM", kMediumSizes },
  { "LARGE",  kLargeSizes },
};

std::map<std::string, std::pair<benchmark_func_t, std::string>> g_str_to_func;

std::mutex g_map_lock;

static struct option g_long_options[] =
{
  {"bionic_cpu", required_argument, nullptr, 'c'},
  {"bionic_xml", required_argument, nullptr, 'x'},
  {"bionic_iterations", required_argument, nullptr, 'i'},
  {"bionic_extra", required_argument, nullptr, 'a'},
  {"help", no_argument, nullptr, 'h'},
  {nullptr, 0, nullptr, 0},
};

typedef std::vector<std::vector<int64_t>> args_vector_t;

void Usage() {
  printf("Usage:\n");
  printf("bionic_benchmarks [--bionic_cpu=<cpu_to_isolate>]\n");
  printf("                  [--bionic_xml=<path_to_xml>]\n");
  printf("                  [--bionic_iterations=<num_iter>]\n");
  printf("                  [--bionic_extra=\"<fn_name> <arg1> <arg 2> ...\"]\n");
  printf("                  [<Google benchmark flags>]\n");
  printf("Google benchmark flags:\n");

  int fake_argc = 2;
  char argv0[] = "bionic_benchmarks";
  char argv1[] = "--help";
  char* fake_argv[3] {argv0, argv1, nullptr};
  benchmark::Initialize(&fake_argc, fake_argv);
  exit(1);
}

// This function removes any bionic benchmarks command line arguments by checking them
// against g_long_options. It fills new_argv with the filtered args.
void SanitizeOpts(int argc, char** argv, std::vector<char*>* new_argv) {
  // TO THOSE ADDING OPTIONS: This currently doesn't support optional arguments.
  (*new_argv)[0] = argv[0];
  for (int i = 1; i < argc; ++i) {
    char* optarg = argv[i];
    size_t opt_idx = 0;

    // Iterate through g_long_options until either we hit the end or we have a match.
    for (opt_idx = 0; g_long_options[opt_idx].name &&
                      strncmp(g_long_options[opt_idx].name, optarg + 2,
                              strlen(g_long_options[opt_idx].name)); ++opt_idx) {
    }

    if (!g_long_options[opt_idx].name) {
      new_argv->push_back(optarg);
    } else {
      if (g_long_options[opt_idx].has_arg == required_argument) {
        // If the arg was passed in with an =, it spans one char *.
        // Otherwise, we skip a spot for the argument.
        if (!strchr(optarg, '=')) {
          i++;
        }
      }
    }
  }
  new_argv->push_back(nullptr);
}

bench_opts_t ParseOpts(int argc, char** argv) {
  bench_opts_t opts;
  int opt;
  int option_index = 0;

  // To make this parser handle the benchmark options silently:
  extern int opterr;
  opterr = 0;

  while ((opt = getopt_long(argc, argv, "c:x:i:a:h", g_long_options, &option_index)) != -1) {
    if (opt == -1) {
      break;
    }
    switch (opt) {
      case 'c':
        if (*optarg) {
          char* check_null;
          opts.cpu_to_lock = strtol(optarg, &check_null, 10);
          if (*check_null) {
            errx(1, "ERROR: Args %s is not a valid integer.", optarg);
          }
        } else {
          printf("ERROR: no argument specified for bionic_cpu\n");
          Usage();
        }
        break;
      case 'x':
        if (*optarg) {
          opts.xmlpath = optarg;
        } else {
          printf("ERROR: no argument specified for bionic_xml\n");
          Usage();
        }
        break;
      case 'a':
        if (*optarg) {
          opts.extra_benchmarks.push_back(optarg);
        } else {
          printf("ERROR: no argument specified for bionic_extra\n");
          Usage();
        }
        break;
      case 'i':
        if (*optarg){
          char* check_null;
          opts.num_iterations = strtol(optarg, &check_null, 10);
          if (*check_null != '\0' or opts.num_iterations < 0) {
            errx(1, "ERROR: Args %s is not a valid number of iterations.", optarg);
          }
        } else {
          printf("ERROR: no argument specified for bionic_iterations\n");
          Usage();
        }
        break;
      case 'h':
        Usage();
        break;
      case '?':
        break;
      default:
        exit(1);
    }
  }
  return opts;
}

// This is a wrapper for every function call for per-benchmark cpu pinning.
void LockAndRun(benchmark::State& state, benchmark_func_t func_to_bench, int cpu_to_lock) {
  if (cpu_to_lock >= 0) LockToCPU(cpu_to_lock);

  // To avoid having to link against Google benchmarks in libutil,
  // benchmarks are kept without parameter information, necessitating this cast.
  reinterpret_cast<void(*) (benchmark::State&)>(func_to_bench)(state);
}

static constexpr char kOnebufManualStr[] = "AT_ONEBUF_MANUAL_ALIGN_";
static constexpr char kTwobufManualStr[] = "AT_TWOBUF_MANUAL_ALIGN1_";

static bool ParseOnebufManualStr(std::string& arg, args_vector_t* to_populate) {
  // The format of this is:
  //   AT_ONEBUF_MANUAL_ALIGN_XX_SIZE_YY
  // Where:
  //   XX is the alignment
  //   YY is the size
  // The YY size can be either a number or a string representing the pre-defined
  // sets of values:
  //   SMALL (for values between 1 and 256)
  //   MEDIUM (for values between 512 and 128KB)
  //   LARGE (for values between 256KB and 2048KB)
  int64_t align;
  int64_t size;
  char sizes[32] = { 0 };
  int ret;

  ret = sscanf(arg.c_str(), "AT_ONEBUF_MANUAL_ALIGN_%" SCNd64 "_SIZE_%" SCNd64,
               &align, &size);
  if (ret == 1) {
    ret = sscanf(arg.c_str(), "AT_ONEBUF_MANUAL_ALIGN_%" SCNd64 "_SIZE_"
                              "%" STRINGFY(sizeof(sizes)-1) "s", &align, sizes);
  }
  if (ret != 2) {
    return false;
  }

  // Verify the alignment is powers of 2.
  if (align != 0 && (align & (align - 1)) != 0) {
    return false;
  }

  auto sit = kSizes.find(sizes);
  if (sit == kSizes.cend()) {
    to_populate->push_back({size, align});
  } else {
    for (auto ssize : sit->second) {
      to_populate->push_back({ssize, align});
    }
  }
  return true;
}

static bool ParseTwobufManualStr(std::string& arg, args_vector_t* to_populate) {
  // The format of this is:
  //   AT_TWOBUF_MANUAL_ALIGN1_XX_ALIGN2_YY_SIZE_ZZ
  // Where:
  //   XX is the alignment of the first argument
  //   YY is the alignment of the second argument
  //   ZZ is the size
  // The ZZ size can be either a number or a string representing the pre-defined
  // sets of values:
  //   SMALL (for values between 1 and 256)
  //   MEDIUM (for values between 512 and 128KB)
  //   LARGE (for values between 256KB and 2048KB)
  int64_t align1;
  int64_t align2;
  int64_t size;
  char sizes[32] = { 0 };
  int ret;

  ret = sscanf(arg.c_str(), "AT_TWOBUF_MANUAL_ALIGN1_%" SCNd64 "_ALIGN2_%" SCNd64 "_SIZE_%" SCNd64,
                            &align1, &align2, &size);
  if (ret == 2) {
    ret = sscanf(arg.c_str(), "AT_TWOBUF_MANUAL_ALIGN1_%" SCNd64 "_ALIGN2_%" SCNd64 "_SIZE_"
                               "%" STRINGFY(sizeof(sizes)-1) "s",
                               &align1, &align2, sizes);
  }
  if (ret != 3) {
    return false;
  }

  // Verify the alignments are powers of 2.
  if ((align1 != 0 && (align1 & (align1 - 1)) != 0)
      || (align2 != 0 && (align2 & (align2 - 1)) != 0)) {
    return false;
  }

  auto sit = kSizes.find(sizes);
  if (sit == kSizes.cend()) {
    to_populate->push_back({size, align1, align2});
  } else {
    for (auto ssize : sit->second) {
      to_populate->push_back({ssize, align1, align2});
    }
  }
  return true;
}

args_vector_t* ResolveArgs(args_vector_t* to_populate, std::string args,
                           std::map<std::string, args_vector_t>& args_shorthand) {
  // args is either a space-separated list of ints, a macro name, or
  // special free form macro.
  // To ease formatting in XML files, args is left and right trimmed.
  if (args_shorthand.count(args)) {
    return &args_shorthand[args];
  }
  // Check for free form macro.
  if (android::base::StartsWith(args, kOnebufManualStr)) {
    if (!ParseOnebufManualStr(args, to_populate)) {
      errx(1, "ERROR: Bad format of macro %s, should be AT_ONEBUF_MANUAL_ALIGN_XX_SIZE_YY",
           args.c_str());
    }
    return to_populate;
  } else if (android::base::StartsWith(args, kTwobufManualStr)) {
    if (!ParseTwobufManualStr(args, to_populate)) {
      errx(1,
           "ERROR: Bad format of macro %s, should be AT_TWOBUF_MANUAL_ALIGN1_XX_ALIGNE2_YY_SIZE_ZZ",
           args.c_str());
    }
    return to_populate;
  }

  to_populate->push_back(std::vector<int64_t>());
  std::stringstream sstream(args);
  std::string argstr;
  while (sstream >> argstr) {
    char* check_null;
    int converted = static_cast<int>(strtol(argstr.c_str(), &check_null, 10));
    if (*check_null) {
      errx(1, "ERROR: Args str %s contains an invalid macro or int.", args.c_str());
    }
    (*to_populate)[0].push_back(converted);
  }
  return to_populate;
}

void RegisterGoogleBenchmarks(bench_opts_t primary_opts, bench_opts_t secondary_opts,
                              const std::string& fn_name, args_vector_t* run_args) {
  if (g_str_to_func.find(fn_name) == g_str_to_func.end()) {
    errx(1, "ERROR: No benchmark for function %s", fn_name.c_str());
  }
  long iterations_to_use = primary_opts.num_iterations ? primary_opts.num_iterations :
                                                         secondary_opts.num_iterations;
  int cpu_to_use = -1;
  if (primary_opts.cpu_to_lock >= 0) {
    cpu_to_use = primary_opts.cpu_to_lock;

  } else if (secondary_opts.cpu_to_lock >= 0) {
    cpu_to_use = secondary_opts.cpu_to_lock;
  }

  benchmark_func_t benchmark_function = g_str_to_func.at(fn_name).first;
  for (const std::vector<int64_t>& args : (*run_args)) {
    auto registration = benchmark::RegisterBenchmark(fn_name.c_str(), LockAndRun,
                                                     benchmark_function,
                                                     cpu_to_use)->Args(args);
    if (iterations_to_use > 0) {
      registration->Iterations(iterations_to_use);
    }
  }
}

void RegisterCliBenchmarks(bench_opts_t cmdline_opts,
                           std::map<std::string, args_vector_t>& args_shorthand) {
  // Register any of the extra benchmarks that were specified in the options.
  args_vector_t arg_vector;
  args_vector_t* run_args = &arg_vector;
  for (const std::string& extra_fn : cmdline_opts.extra_benchmarks) {
    android::base::Trim(extra_fn);
    size_t first_space_pos = extra_fn.find(' ');
    std::string fn_name = extra_fn.substr(0, first_space_pos);
    std::string cmd_args;
    if (first_space_pos != std::string::npos) {
      cmd_args = extra_fn.substr(extra_fn.find(' ') + 1);
    } else {
      cmd_args = "";
    }
    run_args = ResolveArgs(run_args, cmd_args, args_shorthand);
    RegisterGoogleBenchmarks(bench_opts_t(), cmdline_opts, fn_name, run_args);

    run_args = &arg_vector;
    arg_vector.clear();
  }
}

int RegisterXmlBenchmarks(bench_opts_t cmdline_opts,
                          std::map<std::string, args_vector_t>& args_shorthand) {
  // Structure of the XML file:
  // - Element "fn"           Function to benchmark.
  // - - Element "iterations" Number of iterations to run. Leaving this blank uses
  //                          Google benchmarks' convergence heuristics.
  // - - Element "cpu"        CPU to isolate to, if any.
  // - - Element "args"       Whitespace-separated list of per-function integer arguments, or
  //                          one of the macros defined in util.h.
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(cmdline_opts.xmlpath.c_str()) != tinyxml2::XML_SUCCESS) {
    doc.PrintError();
    return doc.ErrorID();
  }

  // Read and register the functions.
  tinyxml2::XMLNode* fn = doc.FirstChildElement("fn");
  while (fn) {
    if (fn == fn->ToComment()) {
      // Skip comments.
      fn = fn->NextSibling();
      continue;
    }

    auto fn_elem = fn->FirstChildElement("name");
    if (!fn_elem) {
      errx(1, "ERROR: Malformed XML entry: missing name element.");
    }
    std::string fn_name = fn_elem->GetText();
    if (fn_name.empty()) {
      errx(1, "ERROR: Malformed XML entry: error parsing name text.");
    }
    auto* xml_args = fn->FirstChildElement("args");
    args_vector_t arg_vector;
    args_vector_t* run_args = ResolveArgs(&arg_vector,
                                          xml_args ? android::base::Trim(xml_args->GetText()) : "",
                                          args_shorthand);

    // XML values for CPU and iterations take precedence over those passed in via CLI.
    bench_opts_t xml_opts{};
    auto* num_iterations_elem = fn->FirstChildElement("iterations");
    if (num_iterations_elem) {
      int temp;
      num_iterations_elem->QueryIntText(&temp);
      xml_opts.num_iterations = temp;
    }
    auto* cpu_to_lock_elem = fn->FirstChildElement("cpu");
    if (cpu_to_lock_elem) {
      int temp;
      cpu_to_lock_elem->QueryIntText(&temp);
      xml_opts.cpu_to_lock = temp;
    }

    RegisterGoogleBenchmarks(xml_opts, cmdline_opts, fn_name, run_args);

    fn = fn->NextSibling();
  }
  return 0;
}

static void SetArgs(const std::vector<int>& sizes, args_vector_t* args) {
  for (int size : sizes) {
    args->push_back({size});
  }
}

static void SetArgs(const std::vector<int>& sizes, int align, args_vector_t* args) {
  for (int size : sizes) {
    args->push_back({size, align});
  }
}


static void SetArgs(const std::vector<int>& sizes, int align1, int align2, args_vector_t* args) {
  for (int size : sizes) {
    args->push_back({size, align1, align2});
  }
}

static args_vector_t GetArgs(const std::vector<int>& sizes) {
  args_vector_t args;
  SetArgs(sizes, &args);
  return args;
}

static args_vector_t GetArgs(const std::vector<int>& sizes, int align) {
  args_vector_t args;
  SetArgs(sizes, align, &args);
  return args;
}

static args_vector_t GetArgs(const std::vector<int>& sizes, int align1, int align2) {
  args_vector_t args;
  SetArgs(sizes, align1, align2, &args);
  return args;
}

std::map<std::string, args_vector_t> GetShorthand() {
  std::vector<int> all_sizes(kSmallSizes);
  all_sizes.insert(all_sizes.end(), kMediumSizes.begin(), kMediumSizes.end());
  all_sizes.insert(all_sizes.end(), kLargeSizes.begin(), kLargeSizes.end());

  std::map<std::string, args_vector_t> args_shorthand {
    {"AT_COMMON_SIZES", GetArgs(kCommonSizes)},
    {"AT_SMALL_SIZES", GetArgs(kSmallSizes)},
    {"AT_MEDIUM_SIZES", GetArgs(kMediumSizes)},
    {"AT_LARGE_SIZES", GetArgs(kLargeSizes)},
    {"AT_ALL_SIZES", GetArgs(all_sizes)},

    {"AT_ALIGNED_ONEBUF", GetArgs(kCommonSizes, 0)},
    {"AT_ALIGNED_ONEBUF_SMALL", GetArgs(kSmallSizes, 0)},
    {"AT_ALIGNED_ONEBUF_MEDIUM", GetArgs(kMediumSizes, 0)},
    {"AT_ALIGNED_ONEBUF_LARGE", GetArgs(kLargeSizes, 0)},
    {"AT_ALIGNED_ONEBUF_ALL", GetArgs(all_sizes, 0)},

    {"AT_ALIGNED_TWOBUF", GetArgs(kCommonSizes, 0, 0)},
    {"AT_ALIGNED_TWOBUF_SMALL", GetArgs(kSmallSizes, 0, 0)},
    {"AT_ALIGNED_TWOBUF_MEDIUM", GetArgs(kMediumSizes, 0, 0)},
    {"AT_ALIGNED_TWOBUF_LARGE", GetArgs(kLargeSizes, 0, 0)},
    {"AT_ALIGNED_TWOBUF_ALL", GetArgs(all_sizes, 0, 0)},

    // Do not exceed 512. that is about the largest number of properties
    // that can be created with the current property area size.
    {"NUM_PROPS", args_vector_t{ {1}, {4}, {16}, {64}, {128}, {256}, {512} }},

    {"MATH_COMMON", args_vector_t{ {0}, {1}, {2}, {3} }},
    {"MATH_SINCOS_COMMON", args_vector_t{ {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7} }},
  };

  args_vector_t args_onebuf;
  args_vector_t args_twobuf;
  for (int size : all_sizes) {
    args_onebuf.push_back({size, 0});
    args_twobuf.push_back({size, 0, 0});
    // Skip alignments on zero sizes.
    if (size == 0) {
      continue;
    }
    for (int align1 = 1; align1 <= 32; align1 <<= 1) {
      args_onebuf.push_back({size, align1});
      for (int align2 = 1; align2 <= 32; align2 <<= 1) {
        args_twobuf.push_back({size, align1, align2});
      }
    }
  }
  args_shorthand.emplace("AT_MANY_ALIGNED_ONEBUF", args_onebuf);
  args_shorthand.emplace("AT_MANY_ALIGNED_TWOBUF", args_twobuf);

  return args_shorthand;
}

static bool FileExists(const std::string& file) {
  struct stat st;
  return stat(file.c_str(), &st) != -1 && S_ISREG(st.st_mode);
}

void RegisterAllBenchmarks(const bench_opts_t& opts,
                           std::map<std::string, args_vector_t>& args_shorthand) {
  for (auto& entry : g_str_to_func) {
    auto& function_info = entry.second;
    args_vector_t arg_vector;
    args_vector_t* run_args = ResolveArgs(&arg_vector, function_info.second,
                                          args_shorthand);
    RegisterGoogleBenchmarks(bench_opts_t(), opts, entry.first, run_args);
  }
}

int main(int argc, char** argv) {
  std::map<std::string, args_vector_t> args_shorthand = GetShorthand();
  bench_opts_t opts = ParseOpts(argc, argv);
  std::vector<char*> new_argv(argc);
  SanitizeOpts(argc, argv, &new_argv);

  if (opts.xmlpath.empty()) {
    // Don't add the default xml file if a user is specifying the tests to run.
    if (opts.extra_benchmarks.empty()) {
      RegisterAllBenchmarks(opts, args_shorthand);
    }
  } else if (!FileExists(opts.xmlpath)) {
    // See if this is a file in the suites directory.
    std::string file(android::base::GetExecutableDirectory() + "/suites/" + opts.xmlpath);
    if (opts.xmlpath[0] == '/' || !FileExists(file)) {
      printf("Cannot find xml file %s: does not exist or is not a file.\n", opts.xmlpath.c_str());
      return 1;
    }
    opts.xmlpath = file;
  }

  if (!opts.xmlpath.empty()) {
    if (int err = RegisterXmlBenchmarks(opts, args_shorthand)) {
      return err;
    }
  }
  RegisterCliBenchmarks(opts, args_shorthand);

  // Set the thread priority to the maximum.
  if (setpriority(PRIO_PROCESS, 0, -20)) {
    perror("Failed to raise priority of process. Are you root?\n");
  }

  int new_argc = new_argv.size();
  benchmark::Initialize(&new_argc, new_argv.data());
  benchmark::RunSpecifiedBenchmarks();
}
