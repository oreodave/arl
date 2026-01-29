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

// standard old appending methods
void vec_append(vec_t *vec, const void *const ptr, u64 size);
void vec_append_byte(vec_t *vec, u8 byte);

// vector-as-a-stack
u8 *vec_pop(vec_t *vec, u64 size);

// Returns pointer to the start of the buffer VEC is currently using to store
// data (either its inline buffer or the heap buffer).
void *vec_data(vec_t *vec);

// Ensure VEC has at least CAPACITY capacity.
void vec_ensure_capacity(vec_t *vec, u64 capacity);

// Ensure VEC has at least SIZE bytes free
void vec_ensure_free(vec_t *vec, u64 size);

// Free the memory associated with the vector
void vec_free(vec_t *vec);

// Reset a vector while preserving any allocations
void vec_reset(vec_t *vec);

// Copy all data from V1 into V2.
void vec_clone(vec_t *v2, vec_t *v1);

// Helper macro to use a vector as a type generic (but homogeneous) container.
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
