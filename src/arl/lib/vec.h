/* vec.h: A dynamically sized array with SBO.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:

 Taken from prick_vec.h: see https://github.com/oreodave/prick.
 */

#ifndef VEC_H
#define VEC_H

#include <assert.h>
#include <stddef.h>

#include <arl/lib/base.h>

#define VEC_INLINE_CAPACITY 32
#define VEC_MULT            2

typedef struct
{
  u64 size, capacity;
  u8 not_inlined;
  union
  {
    void *ptr;
    alignas(max_align_t) u8 inlined[VEC_INLINE_CAPACITY];
  };
} vec_t;
static_assert(sizeof(vec_t) == 64, "Expected sizeof(vec_t) to be 64");

void vec_append(vec_t *vec, const void *const ptr, u64 size);
void vec_append_byte(vec_t *vec, u8 byte);
void *vec_data(vec_t *vec);
void vec_ensure_capacity(vec_t *vec, u64 capacity);
void vec_ensure_free(vec_t *vec, u64 size);
void vec_free(vec_t *vec);
void vec_clone(vec_t *v2, vec_t *v1);

#define VEC_GET(VEC, INDEX, TYPE) (((TYPE *)vec_data(VEC))[INDEX])

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
