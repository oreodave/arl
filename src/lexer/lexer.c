/* lexr.c: Implementation of lexr.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See /include/arl/lexer/lexer.h
 */

#include <ctype.h>
#include <string.h>

#include <arl/lexer/lexer.h>
#include <arl/lexer/token.h>
#include <arl/lib/sv.h>

/// Expected characters in a symbol
static const char *SYMBOL_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&'()*+,-./"
    ":;<=>?@\\^_`{|}~0123456789";

const char *lex_err_to_string(lex_err_t err)
{
  switch (err)
  {
  case LEX_ERR_OK:
    return "OK";
  case LEX_ERR_EXPECTED_SPEECH_MARKS:
    return "EXPECTED_SPEECH_MARKS";
  case LEX_ERR_UNKNOWN_CHAR:
    return "UNKNOWN_CHAR";
  default:
    FAIL("Unexpected lex_err_t value: %d\n", err);
  }
}

/// Prototypes for streams
bool stream_eos(lex_stream_t *stream);
char stream_peek(lex_stream_t *stream);
void stream_advance(lex_stream_t *stream, u64 size);
u64 stream_size(lex_stream_t *stream);

void lex_stream_get_line_col(lex_stream_t *stream, u64 *line, u64 *col)
{
  assert(stream && line && col && "Expected valid pointers.");
  for (u64 i = 0; i < stream->byte; ++i)
  {
    char c = stream->contents.data[i];
    if (c == '\n')
    {
      *line += 1;
      *col = 0;
    }
    else
    {
      *col += 1;
    }
  }
}

/// Prototypes for lexing subroutines
lex_err_t lex_string(lex_stream_t *stream, token_t *ret);
lex_err_t lex_symbol(lex_stream_t *stream, token_t *ret);

lex_err_t lex_stream(token_stream_t *out, lex_stream_t *stream)
{
  assert(out && stream && "Expected valid pointers");
  while (!stream_eos(stream))
  {
    char cur = stream_peek(stream);
    if (isspace(cur))
    {
      while (isspace(cur) && !stream_eos(stream))
      {
        stream_advance(stream, 1);
        cur = stream_peek(stream);
      }
    }
    else if (cur == '"')
    {
      // we make a copy for lex_string to mess with
      token_t ret    = {0};
      lex_err_t perr = lex_string(stream, &ret);
      if (perr)
        return perr;
      vec_append(&out->vec, &ret, sizeof(ret));
    }
    else if (strchr(SYMBOL_CHARS, cur) && !isdigit(cur))
    {
      // we make a copy for lex_symbol to mess with
      token_t ret    = {0};
      lex_err_t perr = lex_symbol(stream, &ret);
      if (perr)
        return perr;

      vec_append(&out->vec, &ret, sizeof(ret));
    }
    else
    {
      return LEX_ERR_UNKNOWN_CHAR;
    }
  }
  return LEX_ERR_OK;
}

lex_err_t lex_string(lex_stream_t *stream, token_t *ret)
{
  // Increment the cursor just past the first speechmark
  stream_advance(stream, 1);
  sv_t string = sv_chop_left(stream->contents, stream->byte);
  string.size = sv_till(string, "\"");

  // If we're at the edge of the stream, there must not have been any
  // speechmarks.
  if (string.size + stream->byte == stream_size(stream))
    return LEX_ERR_EXPECTED_SPEECH_MARKS;

  // `string` is well defined, package and throw it back.
  *ret = token_string(stream->byte - 1, string);
  stream_advance(stream, string.size + 1);
  return LEX_ERR_OK;
}

lex_err_t lex_symbol(lex_stream_t *stream, token_t *ret)
{
  sv_t symbol = sv_chop_left(stream->contents, stream->byte);
  symbol.size = sv_while(symbol, SYMBOL_CHARS);

  // see if symbol is one of the already known symbols
  static_assert(NUM_TOKEN_KNOWNS == 1, "Expected number of TOKEN_KNOWNs");
  for (token_known_t i = 0; i < NUM_TOKEN_KNOWNS; ++i)
  {
    const char *possible_known = token_known_to_cstr(i);
    if (strlen(possible_known) == symbol.size &&
        strncmp(possible_known, symbol.data, symbol.size) == 0)
    {
      // Found a matching known symbol
      *ret = token_known(stream->byte, i);
      goto end;
    }
  }

  // otherwise, it must be a fresh symbol i.e. user defined
  *ret = token_symbol(stream->byte, symbol);
end:
  stream_advance(stream, symbol.size);
  return LEX_ERR_OK;
}

bool stream_eos(lex_stream_t *stream)
{
  return stream->byte >= stream->contents.size;
}

char stream_peek(lex_stream_t *stream)
{
  if (stream_eos(stream))
    return '\0';
  else
    return stream->contents.data[stream->byte];
}

void stream_advance(lex_stream_t *stream, u64 size)
{
  if (stream->byte + size >= stream->contents.size)
    stream->byte = stream->contents.size;
  else
    stream->byte += size;
}

u64 stream_size(lex_stream_t *stream)
{
  return stream->contents.size;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
