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

#include <ftw.h>
#include <stdlib.h>

// Delegation will work in these cases because all the transitive dependencies
// are already 64-bit ready. In particular, we don't have non-O_LARGEFILE
// open (our open is actually open64) and stat and stat64 are the same.
int mkstemp64(char* path) {
  return mkstemp(path);
}
int mkostemp64(char* path, int flags) {
  return mkostemp(path, flags);
}
int mkstemps64(char* path, int suffix_length) {
  return mkstemps(path, suffix_length);
}
int mkostemps64(char* path, int suffix_length, int flags) {
  return mkostemps(path, suffix_length, flags);
}

typedef int (*ftw_fn)(const char*, const struct stat*, int);
typedef int (*nftw_fn)(const char*, const struct stat*, int, struct FTW*);

int ftw64(const char *dirpath,
    int (*fn)(const char*, const struct stat64*, int), int nopenfd) {
  return ftw(dirpath, reinterpret_cast<ftw_fn>(fn), nopenfd);
}

int nftw64(const char * dirpath,
    int (*fn)(const char*, const struct stat64*, int, struct FTW*),
    int nopenfd, int flags) {
  return nftw(dirpath, reinterpret_cast<nftw_fn>(fn), nopenfd, flags);
}
