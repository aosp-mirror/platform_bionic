/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "linker.h"
#include "linker_debug.h"
#include "ba.h"

struct ba_bits {
    unsigned allocated:1;           /* 1 if allocated, 0 if free */
    unsigned order:7;               /* size of the region in ba space */
};

struct ba_info {
    /* start address of the ba space */
    unsigned long base;
    /* total size of the ba space */
    unsigned long size;
    /* number of entries in the ba space */
    int num_entries;
    /* the bitmap for the region indicating which entries are allocated
     * and which are free */
    struct ba_bits *bitmap;
};

#undef min
#define min(a,b) ((a)<(b)?(a):(b))

#define BA_MIN_ALLOC LIBINC
#define BA_MAX_ORDER 128
#define BA_START LIBBASE
#define BA_SIZE (LIBLAST - LIBBASE)

#define BA_IS_FREE(index) (!(ba.bitmap[index].allocated))
#define BA_ORDER(index) ba.bitmap[index].order
#define BA_BUDDY_INDEX(index) ((index) ^ (1 << BA_ORDER(index)))
#define BA_NEXT_INDEX(index) ((index) + (1 << BA_ORDER(index)))
#define BA_OFFSET(index) ((index) * BA_MIN_ALLOC)
#define BA_START_ADDR(index) (BA_OFFSET(index) + ba.base)
#define BA_LEN(index) ((1 << BA_ORDER(index)) * BA_MIN_ALLOC)

static struct ba_bits ba_bitmap[BA_SIZE / BA_MIN_ALLOC];

static struct ba_info ba = {
    .base = BA_START,
    .size = BA_SIZE,
    .bitmap = ba_bitmap,
    .num_entries = sizeof(ba_bitmap)/sizeof(ba_bitmap[0]),
};

void ba_init(void)
{
    int i, index = 0;
    for (i = sizeof(ba.num_entries) * 8 - 1; i >= 0; i--) {
        if (ba.num_entries &  1<<i) {
            BA_ORDER(index) = i;
            index = BA_NEXT_INDEX(index);
        }
    }
}

int ba_free(int index)
{
    int buddy, curr = index;

    /* clean up the bitmap, merging any buddies */
    ba.bitmap[curr].allocated = 0;
    /* find a slots buddy Buddy# = Slot# ^ (1 << order)
     * if the buddy is also free merge them
     * repeat until the buddy is not free or end of the bitmap is reached
     */
    do {
        buddy = BA_BUDDY_INDEX(curr);
        if (BA_IS_FREE(buddy) &&
                BA_ORDER(buddy) == BA_ORDER(curr)) {
            BA_ORDER(buddy)++;
            BA_ORDER(curr)++;
            curr = min(buddy, curr);
        } else {
            break;
        }
    } while (curr < ba.num_entries);

    return 0;
}

static unsigned long ba_order(unsigned long len)
{
    unsigned long i;

    len = (len + BA_MIN_ALLOC - 1) / BA_MIN_ALLOC;
    len--;
    for (i = 0; i < sizeof(len)*8; i++)
        if (len >> i == 0)
            break;
    return i;
}

int ba_allocate(unsigned long len)
{
    int curr = 0;
    int end = ba.num_entries;
    int best_fit = -1;
    unsigned long order = ba_order(len);

    if (order > BA_MAX_ORDER)
        return -1;

    /* look through the bitmap:
     *  if you find a free slot of the correct order use it
     *  otherwise, use the best fit (smallest with size > order) slot
     */
    while (curr < end) {
        if (BA_IS_FREE(curr)) {
            if (BA_ORDER(curr) == (unsigned char)order) {
                /* set the not free bit and clear others */
                best_fit = curr;
                break;
            }
            if (BA_ORDER(curr) > (unsigned char)order &&
                (best_fit < 0 ||
                 BA_ORDER(curr) < BA_ORDER(best_fit)))
                best_fit = curr;
        }
        curr = BA_NEXT_INDEX(curr);
    }

    /* if best_fit < 0, there are no suitable slots,
     * return an error
     */
    if (best_fit < 0)
        return -1;

    /* now partition the best fit:
     *  split the slot into 2 buddies of order - 1
     *  repeat until the slot is of the correct order
     */
    while (BA_ORDER(best_fit) > (unsigned char)order) {
        int buddy;
        BA_ORDER(best_fit) -= 1;
        buddy = BA_BUDDY_INDEX(best_fit);
        BA_ORDER(buddy) = BA_ORDER(best_fit);
    }
    ba.bitmap[best_fit].allocated = 1;
    return best_fit;
}

unsigned long ba_start_addr(int index)
{
    return BA_START_ADDR(index);
}

unsigned long ba_len(int index)
{
    return BA_LEN(index);
}
