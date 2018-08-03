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

#include <search.h>

static int int_cmp(const void* lhs, const void* rhs) {
  return *reinterpret_cast<const int*>(rhs) - *reinterpret_cast<const int*>(lhs);
}

TEST(search, lfind_lsearch) {
  int xs[10];
  memset(xs, 0, sizeof(xs));
  size_t x_size = 0;

  int needle;

  // lfind(3) can't find '2' in the empty table.
  needle = 2;
  ASSERT_EQ(nullptr, lfind(&needle, xs, &x_size, sizeof(xs[0]), int_cmp));
  ASSERT_EQ(0U, x_size);

  // lsearch(3) will add it.
  ASSERT_EQ(&xs[0], lsearch(&needle, xs, &x_size, sizeof(xs[0]), int_cmp));
  ASSERT_EQ(2, xs[0]);
  ASSERT_EQ(1U, x_size);

  // And then lfind(3) can find it.
  ASSERT_EQ(&xs[0], lfind(&needle, xs, &x_size, sizeof(xs[0]), int_cmp));
  ASSERT_EQ(1U, x_size);

  // Inserting a duplicate does nothing (but returns the existing element).
  ASSERT_EQ(&xs[0], lsearch(&needle, xs, &x_size, sizeof(xs[0]), int_cmp));
  ASSERT_EQ(1U, x_size);
}

struct node {
  explicit node(const char* s) : s(strdup(s)) {}

  char* s;
};

static int node_cmp(const void* lhs, const void* rhs) {
  return strcmp(reinterpret_cast<const node*>(lhs)->s, reinterpret_cast<const node*>(rhs)->s);
}

static std::vector<std::string> g_nodes;

static void node_walk(const void* p, VISIT order, int) {
  const node* n = *reinterpret_cast<const node* const*>(p);
  if (order == postorder || order == leaf)  {
    g_nodes.push_back(n->s);
  }
}

static size_t g_free_calls;

static void node_free(void* p) {
  node* n = reinterpret_cast<node*>(p);
  free(n->s);
  ++g_free_calls;
}

TEST(search, tfind_tsearch_twalk_tdestroy) {
  void* root = nullptr;

  node n1("z");
  node n2("a");
  node n3("m");

  // tfind(3) can't find anything in the empty tree.
  ASSERT_EQ(nullptr, tfind(&n1, &root, node_cmp));
  ASSERT_EQ(nullptr, tfind(&n2, &root, node_cmp));
  ASSERT_EQ(nullptr, tfind(&n3, &root, node_cmp));

  // tsearch(3) inserts and returns a pointer to a new node.
  void* i1 = tsearch(&n1, &root, node_cmp);
  ASSERT_NE(nullptr, i1);

  // ...which tfind(3) will then return.
  ASSERT_EQ(i1, tfind(&n1, &root, node_cmp));
  ASSERT_EQ(nullptr, tfind(&n2, &root, node_cmp));
  ASSERT_EQ(nullptr, tfind(&n3, &root, node_cmp));

  // Add the other nodes.
  ASSERT_NE(nullptr, tsearch(&n2, &root, node_cmp));
  ASSERT_NE(nullptr, tsearch(&n3, &root, node_cmp));

  // Use twalk(3) to iterate over the nodes.
  g_nodes.clear();
  twalk(root, node_walk);
  ASSERT_EQ(3U, g_nodes.size());
  ASSERT_EQ("a", g_nodes[0]);
  ASSERT_EQ("m", g_nodes[1]);
  ASSERT_EQ("z", g_nodes[2]);

  // tdestroy(3) removes nodes under a node, calling our callback to destroy each one.
  g_free_calls = 0;
  tdestroy(root, node_free);
  ASSERT_EQ(3U, g_free_calls);
}

struct pod_node {
  explicit pod_node(int i) : i(i) {}
  int i;
};

static int pod_node_cmp(const void* lhs, const void* rhs) {
  return reinterpret_cast<const pod_node*>(rhs)->i - reinterpret_cast<const pod_node*>(lhs)->i;
}

TEST(search, tdelete) {
  void* root = nullptr;

  pod_node n1(123);
  ASSERT_NE(nullptr, tsearch(&n1, &root, pod_node_cmp));

  // tdelete(3) leaks n1.
  pod_node not_there(456);
  ASSERT_EQ(nullptr, tdelete(&not_there, &root, pod_node_cmp));
  ASSERT_NE(nullptr, tdelete(&n1, &root, pod_node_cmp));
}

struct q_node {
  explicit q_node(int i) : i(i) {}

  q_node* next;
  q_node* prev;

  int i;
};

TEST(search, insque_remque) {
  q_node zero(0);
  q_node one(1);
  q_node two(2);

  // Linear (not circular).

  insque(&zero, nullptr);
  insque(&one, &zero);
  insque(&two, &one);

  int expected = 0;
  for (q_node* q = &zero; q != nullptr; q = q->next) {
    ASSERT_EQ(expected, q->i);
    ++expected;
  }
  ASSERT_EQ(3, expected);

  for (q_node* q = &two; q != nullptr; q = q->prev) {
    --expected;
    ASSERT_EQ(expected, q->i);
  }
  ASSERT_EQ(0, expected);

  q_node* head = &zero;

  remque(&one);
  ASSERT_EQ(0, head->i);
  ASSERT_EQ(2, head->next->i);
  ASSERT_EQ(nullptr, head->next->next);

  remque(&two);
  ASSERT_EQ(0, head->i);
  ASSERT_EQ(nullptr, head->next);

  remque(&zero);

  // Circular.

  zero.next = &zero;
  zero.prev = &zero;

  insque(&one, &zero);
  insque(&two, &one);

  ASSERT_EQ(0, head->i);
  ASSERT_EQ(1, head->next->i);
  ASSERT_EQ(2, head->next->next->i);
  ASSERT_EQ(0, head->next->next->next->i);
  ASSERT_EQ(1, head->next->next->next->next->i);
  ASSERT_EQ(2, head->next->next->next->next->next->i);

  remque(&one);
  ASSERT_EQ(0, head->i);
  ASSERT_EQ(2, head->next->i);
  ASSERT_EQ(0, head->next->next->i);
  ASSERT_EQ(2, head->next->next->next->i);

  remque(&two);
  ASSERT_EQ(0, head->i);
  ASSERT_EQ(0, head->next->i);

  remque(&zero);
}

static void AssertEntry(ENTRY* e, const char* expected_key, const char* expected_data) {
  ASSERT_TRUE(e != nullptr);
  ASSERT_STREQ(expected_key, reinterpret_cast<char*>(e->key));
  ASSERT_STREQ(expected_data, reinterpret_cast<char*>(e->data));
}

TEST(search, hcreate_hsearch_hdestroy) {
  ASSERT_NE(0, hcreate(13));

  // Add some initial entries.
  ENTRY* e;
  e = hsearch(ENTRY{.key = const_cast<char*>("a"), .data = const_cast<char*>("A")}, ENTER);
  AssertEntry(e, "a", "A");
  e = hsearch(ENTRY{.key = const_cast<char*>("aa"), .data = const_cast<char*>("B")}, ENTER);
  AssertEntry(e, "aa", "B");
  e = hsearch(ENTRY{.key = const_cast<char*>("aaa"), .data = const_cast<char*>("C")}, ENTER);
  AssertEntry(e, "aaa", "C");

  // Check missing.
  e = hsearch(ENTRY{.key = const_cast<char*>("aaaa"), .data = nullptr}, FIND);
  ASSERT_FALSE(e != nullptr);

  // Check present.
  e = hsearch(ENTRY{.key = const_cast<char*>("aa"), .data = nullptr}, FIND);
  AssertEntry(e, "aa", "B");

  // ENTER with an existing key just returns the existing ENTRY.
  e = hsearch(ENTRY{.key = const_cast<char*>("aa"), .data = const_cast<char*>("X")}, ENTER);
  AssertEntry(e, "aa", "B");
  e->data = const_cast<char*>("X");

  // Check present and updated.
  e = hsearch(ENTRY{.key = const_cast<char*>("aa"), .data = nullptr}, FIND);
  AssertEntry(e, "aa", "X");
  // But other entries stayed the same.
  e = hsearch(ENTRY{.key = const_cast<char*>("a"), .data = nullptr}, FIND);
  AssertEntry(e, "a", "A");
  e = hsearch(ENTRY{.key = const_cast<char*>("aaa"), .data = nullptr}, FIND);
  AssertEntry(e, "aaa", "C");

  hdestroy();
}

TEST(search, hcreate_r_hsearch_r_hdestroy_r) {
  hsearch_data h1 = {};
  ASSERT_EQ(1, hcreate_r(13, &h1));

  hsearch_data h2 = {};
  ASSERT_EQ(1, hcreate_r(128, &h2));

  // Add some initial entries.
  ENTRY* e;
  ASSERT_EQ(1, hsearch_r(ENTRY{.key = const_cast<char*>("a"), .data = const_cast<char*>("A")},
                         ENTER, &e, &h1));
  AssertEntry(e, "a", "A");
  ASSERT_EQ(1, hsearch_r(ENTRY{.key = const_cast<char*>("a"), .data = const_cast<char*>("B")},
                         ENTER, &e, &h2));
  AssertEntry(e, "a", "B");

  // Check missing.
  errno = 0;
  ASSERT_EQ(0, hsearch_r(ENTRY{.key = const_cast<char*>("b"), .data = nullptr}, FIND, &e, &h1));
  ASSERT_EQ(ESRCH, errno);

  // Check present.
  ASSERT_EQ(1, hsearch_r(ENTRY{.key = const_cast<char*>("a"), .data = nullptr}, FIND, &e, &h1));
  AssertEntry(e, "a", "A");
  ASSERT_EQ(1, hsearch_r(ENTRY{.key = const_cast<char*>("a"), .data = nullptr}, FIND, &e, &h2));
  AssertEntry(e, "a", "B");

  // Destroying one doesn't affect the other.
  hdestroy_r(&h1);
  ASSERT_EQ(1, hsearch_r(ENTRY{.key = const_cast<char*>("a"), .data = nullptr}, FIND, &e, &h2));
  AssertEntry(e, "a", "B");
  hdestroy_r(&h2);
}
