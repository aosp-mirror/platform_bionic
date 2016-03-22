/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <string>
#include <vector>

#include <ziparchive/zip_archive.h>
#include <ziparchive/zip_archive_stream_entry.h>
#include <ziparchive/zip_writer.h>

static void usage() {
  fprintf(stderr, "usage: bionic_tests_zipalign ALIGNMENT INPUT_ZIP_FILE OUTPUT_ZIP_FILE\n");
  fprintf(stderr, "  ALIGNMENT:\n");
  fprintf(stderr, "    The new alignment of all entries in the new zip file.\n");
  fprintf(stderr, "  INPUT_ZIP_FILE:\n");
  fprintf(stderr, "    The input zip file that will be read but left unmodified.\n");
  fprintf(stderr, "  OUTPUT_ZIP_FILE:\n");
  fprintf(stderr, "    The output zip file that will be created from the input file.\n");
}

typedef std::pair<ZipEntry*, ZipString*> ZipData;

static bool GetEntries(ZipArchiveHandle handle, std::vector<ZipData>* entries) {
  void* cookie;
  int32_t return_value = StartIteration(handle, &cookie, nullptr, nullptr);
  if (return_value != 0) {
    fprintf(stderr, "Unable to iterate over entries: %s\n", ErrorCodeString(return_value));
    return false;
  }

  ZipEntry entry;
  ZipString name;
  while ((return_value = Next(cookie, &entry, &name)) == 0) {
    entries->push_back(std::make_pair(new ZipEntry(entry), new ZipString(name)));
  }
  if (return_value != -1) {
    fprintf(stderr, "Error while iterating over zip entries: %s\n", ErrorCodeString(return_value));
  } else {
    // Sort by offset.
    std::sort(entries->begin(), entries->end(),
              [](ZipData a, ZipData b) { return a.first->offset < b.first->offset; });
  }

  EndIteration(cookie);
  return return_value == -1;
}

static bool CreateAlignedZip(ZipArchiveHandle& handle, FILE* zip_dst, uint32_t alignment) {
  std::vector<ZipData> entries;
  // We will not free the memory created in entries since the program
  // terminates right after this function is called.
  if (!GetEntries(handle, &entries)) {
    return false;
  }

  ZipWriter writer(zip_dst);

  int32_t error;
  for (auto& entry : entries) {
    ZipEntry* zip_entry = entry.first;
    ZipString* zip_str = entry.second;

    size_t flags = 0;
    if ((zip_entry->method & kCompressDeflated) != 0) {
      flags |= ZipWriter::kCompress;
    }
    std::string zip_name(reinterpret_cast<const char*>(zip_str->name), zip_str->name_length);
    error = writer.StartAlignedEntry(zip_name.c_str(), flags, alignment);
    if (error != 0) {
      fprintf(stderr, "StartAlignedEntry failed: %s\n", ZipWriter::ErrorCodeString(error));
      return false;
    }
    std::unique_ptr<ZipArchiveStreamEntry> stream(
        ZipArchiveStreamEntry::Create(handle, *zip_entry));
    const std::vector<uint8_t>* data;
    while ((data = stream->Read()) != nullptr) {
      error = writer.WriteBytes(data->data(), data->size());
      if (error != 0) {
        fprintf(stderr, "WriteBytes failed: %s\n", ZipWriter::ErrorCodeString(error));
        return false;
      }
    }
    if (!stream->Verify()) {
      fprintf(stderr, "Failed to verify zip stream writer entry.\n");
      return false;
    }
    error = writer.FinishEntry();
    if (error != 0) {
      fprintf(stderr, "FinishEntry failed: %s\n", ZipWriter::ErrorCodeString(error));
    }
  }

  error = writer.Finish();
  if (error != 0) {
    fprintf(stderr, "Finish failed: %s\n", ZipWriter::ErrorCodeString(error));
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    usage();
    return 1;
  }

  char* end;
  unsigned long int alignment = strtoul(argv[1], &end, 10);
  if ((alignment == ULONG_MAX && errno == ERANGE) || *end != '\0') {
    fprintf(stderr, "ALIGNMENT value is not a valid number: %s\n", argv[1]);
    usage();
    return 1;
  }
  if (((alignment - 1) & alignment) != 0) {
    fprintf(stderr, "ALIGNMENT value is not a power of 2: %s\n", argv[1]);
    return 1;
  }

  ZipArchiveHandle handle;

  int32_t return_value = OpenArchive(argv[2], &handle);
  if (return_value != 0) {
    CloseArchive(handle);
    fprintf(stderr, "Unable to open '%s': %s\n", argv[2], ErrorCodeString(return_value));
    return false;
  }

  FILE* zip_dst = fopen(argv[3], "we");
  if (zip_dst == nullptr) {
    fprintf(stderr, "Unable to create '%s': %s\n", argv[3], strerror(errno));
    return 1;
  }

  bool success = CreateAlignedZip(handle, zip_dst, static_cast<uint32_t>(alignment));

  CloseArchive(handle);
  fclose(zip_dst);

  return success ? 0 : 1;
}
