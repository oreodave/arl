/* token.h: General definition of tokens, and a sequence of them.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <arl/lib/base.h>
#include <arl/lib/sv.h>
#include <arl/lib/vec.h>

/// Types of tokens
typedef enum
{
  TOKEN_TYPE_KNOWN = 0,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_STRING,

  NUM_TOKEN_TYPES,
} token_type_t;

/// Known symbols which later stages would benefit from.
typedef enum
{
  TOKEN_KNOWN_PUTS,
  NUM_TOKEN_KNOWNS,
} token_known_t;

const char *token_known_to_cstr(token_known_t);

/// Tokens are a tagged union
typedef struct
{
  u64 byte_location;
  token_type_t type;
  union
  {
    token_known_t as_known;
    sv_t as_symbol;
    sv_t as_string;
  };
} token_t;

token_t token_known(u64 byte, token_known_t known);
token_t token_symbol(u64 byte, sv_t symbol);
token_t token_string(u64 byte, sv_t string);
void token_print(FILE *fp, token_t *token);

/// Sequence of tokens
typedef struct
{
  vec_t vec;
} token_stream_t;

void token_stream_free(token_stream_t *token);
void token_stream_print(FILE *fp, token_stream_t *token);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
