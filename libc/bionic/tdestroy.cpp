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

#define _SEARCH_PRIVATE
#include <search.h>
#include <stdlib.h>

// Destroy a tree and free all allocated resources.
// This is a GNU extension, not available from BSD.
void tdestroy(void* root, void (*destroy_func)(void*)) {
  node_t* root_node = (node_t*) root;
  if (root_node == NULL) {
    return;
  }
  if (root_node->llink) {
    tdestroy(root_node->llink, destroy_func);
  }
  if (root_node->rlink) {
    tdestroy(root_node->rlink, destroy_func);
  }
  (*destroy_func)(root_node->key);
  free(root);
}
