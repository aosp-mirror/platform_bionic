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

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "private/bionic_macros.h"

template<typename T>
struct LinkedListEntry {
  LinkedListEntry<T>* next;
  T* element;
};

/*
 * Represents linked list of objects of type T
 */
template<typename T, typename Allocator>
class LinkedList {
 public:
  LinkedList() : head_(nullptr) {}

  void push_front(T* const element) {
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = head_;
    new_entry->element = element;
    head_ = new_entry;
  }

  void clear() {
    while (head_ != nullptr) {
      LinkedListEntry<T>* p = head_;
      head_ = head_->next;
      Allocator::free(p);
    }
  }

  template<typename F>
  void for_each(F&& action) {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (e->element != nullptr) {
        action(e->element);
      }
    }
  }

  template<typename F>
  void remove_if(F&& predicate) {
    LinkedListEntry<T>* e = head_;
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (e->element != nullptr && predicate(e->element)) {
        e->element = nullptr;
      }
    }
  }

 private:
  LinkedListEntry<T>* head_;
  DISALLOW_COPY_AND_ASSIGN(LinkedList);
};

#endif // __LINKED_LIST_H
