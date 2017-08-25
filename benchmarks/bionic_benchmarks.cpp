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
#include <math.h>
#include <sys/resource.h>

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <benchmark/benchmark.h>
#include <tinyxml2.h>
#include "util.h"

#if defined(__ANDROID__)
static constexpr const char* kDefaultSuite="full.xml";
#else
static constexpr const char* kDefaultSuite="host.xml";
#endif

std::map<std::string, benchmark_func_t> g_str_to_func;

std::mutex g_map_lock;

static struct option g_long_options[] =
{
  {"bionic_cpu", required_argument, 0, 'c'},
  {"bionic_xml", required_argument, 0, 'x'},
  {"bionic_iterations", required_argument, 0, 'i'},
  {"bionic_extra", required_argument, 0, 'a'},
  {"help", no_argument, 0, 'h'},
  {0, 0, 0, 0},
};

typedef std::vector<std::vector<int>> args_vector_t;

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
  char* fake_argv[3] {argv0, argv1, NULL};
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
  new_argv->push_back(0);
}

bench_opts_t ParseOpts(int argc, char** argv) {
  bench_opts_t opts;
  int opt;
  int option_index = 0;

  opts.cpu_to_lock = LONG_MAX;
  opts.num_iterations = 0;

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
void LockAndRun(benchmark::State& state, benchmark_func_t func_to_bench, long cpu_to_lock) {
  if (cpu_to_lock != LONG_MAX) LockToCPU(cpu_to_lock);
  // To avoid having to link against Google benchmarks in libutil,
  // benchmarks are kept without parameter information, necessitating this cast.
  reinterpret_cast<void(*) (benchmark::State&)>(func_to_bench)(state);
}

args_vector_t* ResolveArgs(args_vector_t* to_populate, std::string args,
                           std::map<std::string, args_vector_t>& args_shorthand) {
  // args is either a space-separated list of ints or a macro name.
  // To ease formatting in XML files, args is left and right trimmed.
  if (args_shorthand.count(args)) {
    return &args_shorthand[args];
  }
  to_populate->push_back(std::vector<int>());
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
                         std::string fn_name, args_vector_t* run_args) {
  if (g_str_to_func.find(fn_name) == g_str_to_func.end()) {
    errx(1, "ERROR: No benchmark for function %s", fn_name.c_str());
  }
  long iterations_to_use = primary_opts.num_iterations ? primary_opts.num_iterations :
                                                         secondary_opts.num_iterations;
  int cpu_to_use = INT_MAX;
  if (primary_opts.cpu_to_lock != INT_MAX) {
    cpu_to_use = primary_opts.cpu_to_lock;

  } else if (secondary_opts.cpu_to_lock != INT_MAX) {
    cpu_to_use = secondary_opts.cpu_to_lock;
  }

  for (std::vector<int> args : (*run_args)) {
    auto registration = benchmark::RegisterBenchmark(fn_name.c_str(), LockAndRun,
                                                     g_str_to_func.at(fn_name),
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
  for (std::string extra_fn : cmdline_opts.extra_benchmarks) {
    android::base::Trim(extra_fn);
    size_t first_space_pos = extra_fn.find(" ");
    std::string fn_name = extra_fn.substr(0, first_space_pos);
    std::string cmd_args;
    if (first_space_pos != std::string::npos) {
      cmd_args = extra_fn.substr(extra_fn.find(" ") + 1);
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
    } else {
      xml_opts.num_iterations = 0;
    }
    auto* cpu_to_lock_elem = fn->FirstChildElement("cpu");
    if (cpu_to_lock_elem) {
      int temp;
      cpu_to_lock_elem->QueryIntText(&temp);
      xml_opts.cpu_to_lock = temp;
    } else {
      xml_opts.cpu_to_lock = INT_MAX;
    }

    RegisterGoogleBenchmarks(xml_opts, cmdline_opts, fn_name, run_args);

    fn = fn->NextSibling();
  }
  return 0;
}

std::map<std::string, args_vector_t> GetShorthand() {
  std::map<std::string, args_vector_t> args_shorthand {
    {"AT_ALIGNED_TWOBUF", args_vector_t{ {8, 0, 0},
                                         {64, 0, 0},
                                         {512, 0, 0},
                                         {1 * KB, 0, 0},
                                         {8 * KB, 0, 0},
                                         {16 * KB, 0, 0},
                                         {32 * KB, 0, 0},
                                         {64 * KB, 0, 0} }},
    {"AT_ALIGNED_ONEBUF", args_vector_t{ {(8), 0},
                                         {(64), 0},
                                         {(512), 0},
                                         {(1*KB), 0},
                                         {(8*KB), 0},
                                         {(16*KB), 0},
                                         {(32*KB), 0},
                                         {(64*KB), 0}}},

    {"AT_COMMON_SIZES", args_vector_t{ {8}, {64}, {512}, {1*KB}, {8*KB}, {16*KB},
                                                       {32*KB}, {64*KB}}},

    // Do not exceed 512. that is about the largest number of properties
    // that can be created with the current property area size.
    {"NUM_PROPS", args_vector_t{ {1}, {4}, {16}, {64}, {128}, {256}, {512} }},

    {"MATH_COMMON", args_vector_t{ {0}, {1}, {2}, {3} }}
  };
  for (int i = 1; i < 15; i++) {
    int align = pow(2, i);
    std::stringstream sstream;
    sstream << "AT_" << align << "_ALIGN_TWOBUF";
    args_shorthand.emplace(sstream.str(),
                           args_vector_t{ {8, align, align},
                                          {64, align, align},
                                          {512, align, align},
                                          {1 * KB, align, align},
                                          {8 * KB, align, align},
                                          {16 * KB, align, align},
                                          {32 * KB, align, align},
                                          {64 * KB, align, align} });
    sstream.str("");
    sstream << "AT_" << align << "_ALIGN_ONEBUF";
    args_shorthand.emplace(sstream.str(),
                            args_vector_t{ {(8), align},
                                           {(64), align},
                                           {(512), align},
                                           {(1*KB), align},
                                           {(8*KB), align},
                                           {(16*KB), align},
                                           {(32*KB), align},
                                           {(64*KB), align} });
    sstream.str("");
  }
  return args_shorthand;
}

static bool FileExists(const std::string& file) {
  struct stat st;
  return stat(file.c_str(), &st) != -1 && S_ISREG(st.st_mode);
}

int main(int argc, char** argv) {
  std::map<std::string, args_vector_t> args_shorthand = GetShorthand();
  bench_opts_t opts = ParseOpts(argc, argv);
  std::vector<char*> new_argv(argc);
  SanitizeOpts(argc, argv, &new_argv);

  if (opts.xmlpath.empty()) {
    // Don't add the default xml file if a user is specifying the tests to run.
    if (opts.extra_benchmarks.empty()) {
      // Try and use the default.
      opts.xmlpath = android::base::GetExecutableDirectory() + "/suites/" + kDefaultSuite;
      if (!FileExists(opts.xmlpath)) {
        printf("Cannot find default xml file %s\n", kDefaultSuite);
        return 1;
      }
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
