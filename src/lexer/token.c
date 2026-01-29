/* token.c: Implementation of TOKEN constructor/destructor functions
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See /include/arl/lexer/token.h.
 */

#include <arl/lexer/token.h>
#include <arl/lib/base.h>
#include <arl/lib/vec.h>

const char *token_known_to_cstr(token_known_t known)
{
  switch (known)
  {
  case TOKEN_KNOWN_PUTS:
    return "puts";
  default:
    FAIL("Unexpected TOKEN_KNOWN value: %d\n", known);
  }
}

token_t token_known(u64 byte, token_known_t known)
{
  return (token_t){
      .byte_location = byte,
      .type          = TOKEN_TYPE_KNOWN,
      .as_known      = known,
  };
}

token_t token_string(u64 byte, sv_t string)
{
  return (token_t){
      .byte_location = byte,
      .type          = TOKEN_TYPE_STRING,
      .as_string     = string,
  };
}

token_t token_symbol(u64 byte, sv_t symbol)
{
  return (token_t){
      .byte_location = byte,
      .type          = TOKEN_TYPE_SYMBOL,
      .as_symbol     = symbol,
  };
}

void token_print(FILE *fp, token_t *token)
{
  if (!token)
  {
    fprintf(fp, "NIL");
    return;
  }
  switch (token->type)
  {
  case TOKEN_TYPE_KNOWN:
    fprintf(fp, "KNOWN(%s)", token_known_to_cstr(token->as_known));
    break;
  case TOKEN_TYPE_SYMBOL:
    fprintf(fp, "SYMBOL(" PR_SV ")", SV_FMT(token->as_symbol));
    break;
  case TOKEN_TYPE_STRING:
    fprintf(fp, "STRING(" PR_SV ")", SV_FMT(token->as_string));
    break;
  case NUM_TOKEN_TYPES:
  default:
    FAIL("Unexpected token type: %d\n", token->type);
  }
}

void token_stream_print(FILE *fp, token_stream_t *token)
{
  if (!token)
  {
    fprintf(fp, "{}");
    return;
  }
  fprintf(fp, "{");
  if (token->vec.size == 0)
  {
    fprintf(fp, "}\n");
    return;
  }

  fprintf(fp, "\n");
  for (u64 i = 0; i < token->vec.size / sizeof(token_t); ++i)
  {
    token_t item = VEC_GET(&token->vec, i, token_t);
    fprintf(fp, "\t[%lu]: ", i);
    token_print(fp, &item);
    fprintf(fp, "\n");
  }
  fprintf(fp, "}");
}

void token_stream_free(token_stream_t *stream)
{
  // we can free the vector itself and we're done
  vec_free(&stream->vec);
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
