/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdlib.h>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "../linked_list.h"

namespace {

bool alloc_called = false;
bool free_called = false;

class LinkedListTestAllocator {
 public:
  typedef LinkedListEntry<const char> entry_t;

  static entry_t* alloc() {
    alloc_called = true;
    return reinterpret_cast<entry_t*>(::malloc(sizeof(entry_t)));
  }

  static void free(entry_t* p) {
    free_called = true;
    ::free(p);
  }
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(LinkedListTestAllocator);
};

typedef LinkedList<const char, LinkedListTestAllocator> test_list_t;

std::string test_list_to_string(test_list_t& list) {
  std::stringstream ss;
  list.for_each([&] (const char* c) {
    ss << c;
  });

  return ss.str();
}

};

TEST(linked_list, simple) {
  alloc_called = free_called = false;
  test_list_t list;
  ASSERT_EQ("", test_list_to_string(list));
  ASSERT_TRUE(!alloc_called);
  ASSERT_TRUE(!free_called);
  list.push_front("a");
  ASSERT_TRUE(alloc_called);
  ASSERT_TRUE(!free_called);
  ASSERT_EQ("a", test_list_to_string(list));
  list.push_front("b");
  ASSERT_EQ("ba", test_list_to_string(list));
  list.push_front("c");
  list.push_front("d");
  ASSERT_EQ("dcba", test_list_to_string(list));
  ASSERT_TRUE(alloc_called);
  ASSERT_TRUE(!free_called);
  alloc_called = free_called = false;
  list.remove_if([] (const char* c) {
    return *c == 'c';
  });

  ASSERT_TRUE(!alloc_called);
  ASSERT_TRUE(!free_called);

  ASSERT_EQ("dba", test_list_to_string(list));
  alloc_called = free_called = false;
  list.remove_if([] (const char* c) {
    return *c == '2';
  });
  ASSERT_TRUE(!alloc_called);
  ASSERT_TRUE(!free_called);
  ASSERT_EQ("dba", test_list_to_string(list));
  list.clear();
  ASSERT_TRUE(!alloc_called);
  ASSERT_TRUE(free_called);
  ASSERT_EQ("", test_list_to_string(list));
}
