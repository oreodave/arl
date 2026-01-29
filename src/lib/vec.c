/* vec.c: Vector implementation
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See /include/arl/lib/vec.h

 Taken from prick_vec.h: see https://github.com/oreodave/prick.
 */

#include <arl/lib/base.h>
#include <arl/lib/vec.h>

#include <stdlib.h>
#include <string.h>

void vec_append(vec_t *vec, const void *const ptr, u64 size)
{
  if (!vec || !ptr || !size)
    return;
  vec_ensure_free(vec, size);
  memcpy(&VEC_GET(vec, vec->size, u8), ptr, size);
  vec->size += size;
}

void vec_append_byte(vec_t *vec, u8 byte)
{
  if (!vec)
    return;
  vec_ensure_free(vec, 1);
  VEC_GET(vec, vec->size, u8) = byte;
  ++vec->size;
}

u8 *vec_pop(vec_t *vec, u64 size)
{
  if (!vec || vec->size < size)
    return NULL;
  vec->size -= size;
  return (u8 *)vec_data(vec) + vec->size;
}

void *vec_data(vec_t *vec)
{
  if (!vec)
    return NULL;

  if (vec->not_inlined)
  {
    return vec->ptr;
  }
  else
  {
    return vec->inlined;
  }
}

void vec_ensure_capacity(vec_t *vec, u64 capacity)
{
  if (!vec)
    return;
  if (vec->capacity == 0)
    vec->capacity = VEC_INLINE_CAPACITY;
  if (vec->capacity < capacity)
  {
    vec->capacity = MAX(vec->capacity * VEC_MULT, capacity);
    if (!vec->not_inlined)
    {
      // We were a small buffer, and now we cannot be i.e. we need to allocate
      // on the heap.
      vec->not_inlined = 1;
      void *buffer     = calloc(1, vec->capacity);
      memcpy(buffer, vec->inlined, vec->size);
      memset(vec->inlined, 0, sizeof(vec->inlined));
      vec->ptr = buffer;
    }
    else
    {
      // We're already on the heap, just reallocate.
      vec->ptr = realloc(vec->ptr, vec->capacity);
    }
  }
}

void vec_ensure_free(vec_t *vec, u64 size)
{
  if (!vec)
    return;
  vec_ensure_capacity(vec, vec->size + size);
}

void vec_free(vec_t *vec)
{
  if (!vec)
    return;
  if (vec->not_inlined)
    free(vec->ptr);
  memset(vec, 1, sizeof(*vec));
}

void vec_reset(vec_t *vec)
{
  if (!vec)
    return;
  memset(vec_data(vec), 0, vec->capacity);
  vec->size = 0;
}

void vec_clone(vec_t *v2, vec_t *v1)
{
  if (!v1 || !v2)
    return;
  vec_append(v2, vec_data(v1), v1->size);
}

#undef MAX

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
