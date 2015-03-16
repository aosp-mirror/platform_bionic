/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef BENCHMARKS_BENCHMARK_H_
#define BENCHMARKS_BENCHMARK_H_

#include <regex.h>
#include <stdint.h>

#include <string>
#include <vector>

namespace testing {

class Benchmark {
public:
  Benchmark() {
    List().push_back(this);
  }
  virtual ~Benchmark() {}

  virtual std::string Name() = 0;

  virtual size_t RunAllArgs(std::vector<regex_t*>&) = 0;

  void SetBenchmarkBytesProcessed(uint64_t bytes) { bytes_processed_ += bytes; }
  void StopBenchmarkTiming();
  void StartBenchmarkTiming();

  // Run all of the benchmarks that have registered.
  static size_t RunAll(std::vector<regex_t*>&);

  static std::vector<Benchmark*>& List();

  static int MaxNameColumnWidth();

protected:
  virtual size_t NameColumnWidth() = 0;

  uint64_t bytes_processed_;
  uint64_t total_time_ns_;
  uint64_t start_time_ns_;

  static bool header_printed_;

  static void PrintHeader();
};

template <typename T>
class BenchmarkT : public Benchmark {
public:
  BenchmarkT() {}
  virtual ~BenchmarkT() {}

protected:
  bool ShouldRun(std::vector<regex_t*>&, T arg);
  void RunWithArg(T arg);
  virtual void RunIterations(int, T) = 0;
  virtual std::string GetNameStr(T) = 0;
};

class BenchmarkWithoutArg : public BenchmarkT<void*> {
public:
  BenchmarkWithoutArg() {}
  virtual ~BenchmarkWithoutArg() {}

protected:
  virtual size_t RunAllArgs(std::vector<regex_t*>& regs) override {
    size_t benchmarks_run = 0;
    if (BenchmarkT<void*>::ShouldRun(regs, nullptr)) {
      PrintHeader();
      RunWithArg(nullptr);
      benchmarks_run++;
    }
    return benchmarks_run;
  }

  virtual void RunIterations(int iters, void*) override {
    Run(iters);
  }

  virtual void Run(int) = 0;

  virtual size_t NameColumnWidth() override {
    return Name().size();
  }

  virtual std::string GetNameStr(void *) override;
};

template<typename T>
class BenchmarkWithArg : public BenchmarkT<T> {
public:
  BenchmarkWithArg() {}
  virtual ~BenchmarkWithArg() {}

  BenchmarkWithArg* Arg(T arg) {
    args_.push_back(arg);
    return this;
  }

protected:
  virtual size_t NameColumnWidth() override {
    size_t max = 0;
    for (const auto& arg : args_) {
      max = std::max(max, GetNameStr(arg).size());
    }
    return max;
  }

  std::string GetNameStr(T arg) override;

  virtual size_t RunAllArgs(std::vector<regex_t*>& regs) override {
    size_t benchmarks_run = 0;
    for (T& arg : args_) {
      if (BenchmarkT<T>::ShouldRun(regs, arg)) {
        Benchmark::PrintHeader();
        BenchmarkT<T>::RunWithArg(arg);
        benchmarks_run++;
      }
    }
    return benchmarks_run;
  }

  virtual void RunIterations(int iters, T arg) override {
    Run(iters, arg);
  }

  virtual void Run(int iters, T arg) = 0;

private:
  std::vector<T> args_;
};

}  // namespace testing

#define BENCHMARK_START(f, super_class) \
  class f : public super_class { \
  public: \
    f() {} \
    virtual ~f() {} \
    virtual std::string Name() override { return #f; } \

#define BENCHMARK_NO_ARG(f) \
  BENCHMARK_START(f, ::testing::BenchmarkWithoutArg) \
    virtual void Run(int) override; \
  }; \
  static ::testing::Benchmark* __benchmark_##f = new f()

#define BENCHMARK_WITH_ARG(f, arg_type) \
  BENCHMARK_START(f, ::testing::BenchmarkWithArg<arg_type>) \
    virtual void Run(int, arg_type) override; \
  }; \
  static ::testing::BenchmarkWithArg<arg_type>* __benchmark_##f = (new f())

#endif  // BENCHMARKS_BENCHMARK_H_
