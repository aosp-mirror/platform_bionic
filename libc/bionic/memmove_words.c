/*
 * Copyright (C) 2011 The Android Open Source Project
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
#include <stdint.h>
#include <assert.h>

/*
 * Works like memmove(), except:
 * - if all arguments are at least 32-bit aligned, we guarantee that we
 *   will use operations that preserve atomicity of 32-bit values
 * - if not, we guarantee atomicity of 16-bit values
 *
 * If all three arguments are not at least 16-bit aligned, the behavior
 * of this function is undefined.  (We could remove this restriction by
 * testing for unaligned values and punting to memmove(), but that's
 * not currently useful.)
 *
 * TODO: add loop for 64-bit alignment
 * TODO: use __builtin_prefetch
 * TODO: write an ARM-optimized version
 */
void _memmove_words(void* dest, const void* src, size_t n)
{
    assert((((uintptr_t) dest | (uintptr_t) src | n) & 0x01) == 0);

    char* d = (char*) dest;
    const char* s = (const char*) src;
    size_t copyCount;

    /*
     * If the source and destination pointers are the same, this is
     * an expensive no-op.  Testing for an empty move now allows us
     * to skip a check later.
     */
    if (n == 0 || d == s)
        return;

    /*
     * Determine if the source and destination buffers will overlap if
     * we copy data forward (i.e. *dest++ = *src++).
     *
     * It's okay if the destination buffer starts before the source and
     * there is some overlap, because the reader is always ahead of the
     * writer.
     */
    if (__builtin_expect((d < s) || ((size_t)(d - s) >= n), 1)) {
        /*
         * Copy forward.  We prefer 32-bit loads and stores even for 16-bit
         * data, so sort that out.
         */
        if ((((uintptr_t) d | (uintptr_t) s) & 0x03) != 0) {
            /*
             * Not 32-bit aligned.  Two possibilities:
             * (1) Congruent, we can align to 32-bit by copying one 16-bit val
             * (2) Non-congruent, we can do one of:
             *   a. copy whole buffer as a series of 16-bit values
             *   b. load/store 32 bits, using shifts to ensure alignment
             *   c. just copy the as 32-bit values and assume the CPU
             *      will do a reasonable job
             *
             * We're currently using (a), which is suboptimal.
             */
            if ((((uintptr_t) d ^ (uintptr_t) s) & 0x03) != 0) {
                copyCount = n;
            } else {
                copyCount = 2;
            }
            n -= copyCount;
            copyCount /= sizeof(uint16_t);

            while (copyCount--) {
                *(uint16_t*)d = *(uint16_t*)s;
                d += sizeof(uint16_t);
                s += sizeof(uint16_t);
            }
        }

        /*
         * Copy 32-bit aligned words.
         */
        copyCount = n / sizeof(uint32_t);
        while (copyCount--) {
            *(uint32_t*)d = *(uint32_t*)s;
            d += sizeof(uint32_t);
            s += sizeof(uint32_t);
        }

        /*
         * Check for leftovers.  Either we finished exactly, or we have
         * one remaining 16-bit chunk.
         */
        if ((n & 0x02) != 0) {
            *(uint16_t*)d = *(uint16_t*)s;
        }
    } else {
        /*
         * Copy backward, starting at the end.
         */
        d += n;
        s += n;

        if ((((uintptr_t) d | (uintptr_t) s) & 0x03) != 0) {
            /* try for 32-bit alignment */
            if ((((uintptr_t) d ^ (uintptr_t) s) & 0x03) != 0) {
                copyCount = n;
            } else {
                copyCount = 2;
            }
            n -= copyCount;
            copyCount /= sizeof(uint16_t);

            while (copyCount--) {
                d -= sizeof(uint16_t);
                s -= sizeof(uint16_t);
                *(uint16_t*)d = *(uint16_t*)s;
            }
        }

        /* copy 32-bit aligned words */
        copyCount = n / sizeof(uint32_t);
        while (copyCount--) {
            d -= sizeof(uint32_t);
            s -= sizeof(uint32_t);
            *(uint32_t*)d = *(uint32_t*)s;
        }

        /* copy leftovers */
        if ((n & 0x02) != 0) {
            d -= sizeof(uint16_t);
            s -= sizeof(uint16_t);
            *(uint16_t*)d = *(uint16_t*)s;
        }
    }
}
