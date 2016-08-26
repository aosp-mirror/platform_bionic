/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <math.h>
#include <fenv.h>

template <typename RT, typename T1>
struct data_1_1_t {
  RT expected;
  T1 input;
};

template <typename T1>
struct data_int_1_t {
  int expected;
  T1 input;
};

template <typename T1>
struct data_long_1_t {
  long expected;
  T1 input;
};

template <typename T1>
struct data_llong_1_t {
  long long expected;
  T1 input;
};

template <typename RT, typename T1, typename T2>
struct data_1_2_t {
  RT expected;
  T1 input1;
  T2 input2;
};

template <typename RT1, typename RT2, typename T>
struct data_2_1_t {
  RT1 expected1;
  RT2 expected2;
  T input;
};

template <typename RT1, typename T>
struct data_1_int_1_t {
  RT1 expected1;
  int expected2;
  T input;
};

template <typename RT1, typename T1, typename T2>
struct data_1_int_2_t {
  RT1 expected1;
  int expected2;
  T1 input1;
  T2 input2;
};

template <typename RT, typename T1, typename T2, typename T3>
struct data_1_3_t {
  RT expected;
  T1 input1;
  T2 input2;
  T3 input3;
};

template <typename T> union fp_u;

template <> union fp_u<float> {
  float value;
  struct {
    unsigned frac:23;
    unsigned exp:8;
    unsigned sign:1;
  } bits;
  uint32_t sign_magnitude;
};

template <> union fp_u<double> {
  double value;
  struct {
    unsigned fracl;
    unsigned frach:20;
    unsigned exp:11;
    unsigned sign:1;
  } bits;
  uint64_t sign_magnitude;
};

template <> union fp_u<long double> {
  long double value;
#if defined(__LP64__)
  struct {
    unsigned fracl;
    unsigned fraclm;
    unsigned frachm;
    unsigned frach:16;
    unsigned exp:15;
    unsigned sign:1;
  } bits;
  __int128_t sign_magnitude;
#else
  struct {
      unsigned fracl;
      unsigned frach:20;
      unsigned exp:11;
      unsigned sign:1;
  } bits;
  uint64_t sign_magnitude;
#endif
};

template <typename T>
static inline auto SignAndMagnitudeToBiased(const T& value) -> decltype(fp_u<T>::sign_magnitude) {
  fp_u<T> u;
  u.value = value;
  if (u.bits.sign) {
    return ~u.sign_magnitude + 1;
  } else {
    u.bits.sign = 1;
    return u.sign_magnitude;
  }
}

// Based on the existing googletest implementation, which uses a fixed 4 ulp bound.
template <typename T>
size_t UlpDistance(T lhs, T rhs) {
  const auto biased1 = SignAndMagnitudeToBiased(lhs);
  const auto biased2 = SignAndMagnitudeToBiased(rhs);
  return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
}

template <size_t ULP, typename T>
struct FpUlpEq {
  ::testing::AssertionResult operator()(const char* /* expected_expression */,
                                        const char* /* actual_expression */,
                                        T expected,
                                        T actual) {
    if (!isnan(expected) && !isnan(actual) && UlpDistance(expected, actual) <= ULP) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure()
        << "expected (" << std::hexfloat << expected << ") != actual (" << actual << ")";
  }
};

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double) -> double function like sin(3).
template <size_t ULP, typename RT, typename T, size_t N>
void DoMathDataTest(data_1_1_t<RT, T> (&data)[N], RT f(T)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT> predicate;
  for (size_t i = 0; i < N; ++i) {
    EXPECT_PRED_FORMAT2(predicate,
                        data[i].expected, f(data[i].input)) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double) -> int function like ilogb(3).
template <size_t ULP, typename T, size_t N>
void DoMathDataTest(data_int_1_t<T> (&data)[N], int f(T)) {
  fesetenv(FE_DFL_ENV);
  for (size_t i = 0; i < N; ++i) {
    EXPECT_EQ(data[i].expected, f(data[i].input)) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double) -> long int function like lrint(3).
template <size_t ULP, typename T, size_t N>
void DoMathDataTest(data_long_1_t<T> (&data)[N], long f(T)) {
  fesetenv(FE_DFL_ENV);
  for (size_t i = 0; i < N; ++i) {
    EXPECT_EQ(data[i].expected, f(data[i].input)) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double) -> long long int function like llrint(3).
template <size_t ULP, typename T, size_t N>
void DoMathDataTest(data_llong_1_t<T> (&data)[N], long long f(T)) {
  fesetenv(FE_DFL_ENV);
  for (size_t i = 0; i < N; ++i) {
    EXPECT_EQ(data[i].expected, f(data[i].input)) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the pairs of input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double, double) -> double function like pow(3).
template <size_t ULP, typename RT, typename T1, typename T2, size_t N>
void DoMathDataTest(data_1_2_t<RT, T1, T2> (&data)[N], RT f(T1, T2)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT> predicate;
  for (size_t i = 0; i < N; ++i) {
    EXPECT_PRED_FORMAT2(predicate,
                        data[i].expected, f(data[i].input1, data[i].input2)) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the results are within ULP ulps of the expected values.
// For testing a (double, double*, double*) -> void function like sincos(3).
template <size_t ULP, typename RT1, typename RT2, typename T1, size_t N>
void DoMathDataTest(data_2_1_t<RT1, RT2, T1> (&data)[N], void f(T1, RT1*, RT2*)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT1> predicate1;
  FpUlpEq<ULP, RT2> predicate2;
  for (size_t i = 0; i < N; ++i) {
    RT1 out1;
    RT2 out2;
    f(data[i].input, &out1, &out2);
    EXPECT_PRED_FORMAT2(predicate1, data[i].expected1, out1) << "Failed on element " << i;
    EXPECT_PRED_FORMAT2(predicate2, data[i].expected2, out2) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the results are within ULP ulps of the expected values.
// For testing a (double, double*) -> double function like modf(3).
template <size_t ULP, typename RT1, typename RT2, typename T1, size_t N>
void DoMathDataTest(data_2_1_t<RT1, RT2, T1> (&data)[N], RT1 f(T1, RT2*)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT1> predicate1;
  FpUlpEq<ULP, RT2> predicate2;
  for (size_t i = 0; i < N; ++i) {
    RT1 out1;
    RT2 out2;
    out1 = f(data[i].input, &out2);
    EXPECT_PRED_FORMAT2(predicate1, data[i].expected1, out1) << "Failed on element " << i;
    EXPECT_PRED_FORMAT2(predicate2, data[i].expected2, out2) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the results are within ULP ulps of the expected values.
// For testing a (double, int*) -> double function like frexp(3).
template <size_t ULP, typename RT1, typename T1, size_t N>
void DoMathDataTest(data_1_int_1_t<RT1, T1> (&data)[N], RT1 f(T1, int*)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT1> predicate1;
  for (size_t i = 0; i < N; ++i) {
    RT1 out1;
    int out2;
    out1 = f(data[i].input, &out2);
    EXPECT_PRED_FORMAT2(predicate1, data[i].expected1, out1) << "Failed on element " << i;
    EXPECT_EQ(data[i].expected2, out2) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the input values
// and asserting that the results are within ULP ulps of the expected values.
// For testing a (double, double, int*) -> double function like remquo(3).
template <size_t ULP, typename RT1, typename T1, typename T2, size_t N>
void DoMathDataTest(data_1_int_2_t<RT1, T1, T2> (&data)[N], RT1 f(T1, T2, int*)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT1> predicate1;
  for (size_t i = 0; i < N; ++i) {
    RT1 out1;
    int out2;
    out1 = f(data[i].input1, data[i].input2, &out2);
    EXPECT_PRED_FORMAT2(predicate1, data[i].expected1, out1) << "Failed on element " << i;
    EXPECT_EQ(data[i].expected2, out2) << "Failed on element " << i;
  }
}

// Runs through the array 'data' applying 'f' to each of the pairs of input values
// and asserting that the result is within ULP ulps of the expected value.
// For testing a (double, double, double) -> double function like fma(3).
template <size_t ULP, typename RT, typename T1, typename T2, typename T3, size_t N>
void DoMathDataTest(data_1_3_t<RT, T1, T2, T3> (&data)[N], RT f(T1, T2, T3)) {
  fesetenv(FE_DFL_ENV);
  FpUlpEq<ULP, RT> predicate;
  for (size_t i = 0; i < N; ++i) {
    EXPECT_PRED_FORMAT2(predicate,
                        data[i].expected, f(data[i].input1, data[i].input2, data[i].input3)) << "Failed on element " << i;
  }
}
