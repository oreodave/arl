/* base.h: Basic definitions
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:

 Taken from prick_aliases.h: see https://github.com/oreodave/prick.
 */

#ifndef BASE_H
#define BASE_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

static_assert(sizeof(float) == 4, "f32 requires 4 byte floats");
static_assert(sizeof(double) == 8, "f64 requires 8 byte doubles");
typedef float f32;
typedef double f64;

#define MAX(A, B)  ((A) > (B) ? (A) : (B))
#define MIN(A, B)  ((A) > (B) ? (B) : (A))
#define ARRSIZE(A) ((sizeof(A)) / sizeof((A)[0]))

#define FAIL(...)                 \
  do                              \
  {                               \
    fprintf(stderr, "FAIL: ");    \
    fprintf(stderr, __VA_ARGS__); \
    assert(0);                    \
  } while (0)

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
