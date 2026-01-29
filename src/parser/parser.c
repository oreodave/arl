/* parser.c: Implementation of parser.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See /include/arl/parser/parser.h
 */

#include <ctype.h>
#include <string.h>

#include <arl/lib/sv.h>
#include <arl/parser/ast.h>
#include <arl/parser/parser.h>

/// Expected characters in a symbol
static const char *SYMBOL_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&'()*+,-./"
    ":;<=>?@\\^_`{|}~0123456789";

const char *parse_err_to_string(parse_err_t err)
{
  switch (err)
  {
  case PARSE_ERR_OK:
    return "OK";
  case PARSE_ERR_EXPECTED_SPEECH_MARKS:
    return "EXPECTED_SPEECH_MARKS";
  case PARSE_ERR_UNKNOWN_CHAR:
    return "UNKNOWN_CHAR";
  default:
    FAIL("Unexpected parse_err_t value: %d\n", err);
  }
}

/// Prototypes for streams
bool stream_eos(parse_stream_t *stream);
char stream_peek(parse_stream_t *stream);
void stream_advance(parse_stream_t *stream, u64 size);
u64 stream_size(parse_stream_t *stream);

void parse_stream_get_line_col(parse_stream_t *stream, u64 *line, u64 *col)
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

/// Prototypes for parsing subroutines
parse_err_t parse_string(parse_stream_t *stream, ast_node_t *ret);
parse_err_t parse_symbol(parse_stream_t *stream, ast_node_t *ret);

parse_err_t parse(ast_t *out, parse_stream_t *stream)
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
      // we make a copy for parse_string to mess with
      ast_node_t ret   = {0};
      parse_err_t perr = parse_string(stream, &ret);
      if (perr)
        return perr;
      vec_append(&out->nodes, &ret, sizeof(ret));
    }
    else if (strchr(SYMBOL_CHARS, cur) && !isdigit(cur))
    {
      // we make a copy for parse_symbol to mess with
      ast_node_t ret   = {0};
      parse_err_t perr = parse_symbol(stream, &ret);
      if (perr)
        return perr;

      vec_append(&out->nodes, &ret, sizeof(ret));
    }
    else
    {
      return PARSE_ERR_UNKNOWN_CHAR;
    }
  }
  return PARSE_ERR_OK;
}

parse_err_t parse_string(parse_stream_t *stream, ast_node_t *ret)
{
  // Increment the cursor just past the first speechmark
  stream_advance(stream, 1);
  sv_t string = sv_chop_left(stream->contents, stream->byte);
  string.size = sv_till(string, "\"");

  // If we're at the edge of the stream, there must not have been any
  // speechmarks.
  if (string.size + stream->byte == stream_size(stream))
    return PARSE_ERR_EXPECTED_SPEECH_MARKS;

  // `string` is well defined, package and throw it back.
  *ret = ast_node_string(stream->byte - 1, string);
  stream_advance(stream, string.size + 1);
  return PARSE_ERR_OK;
}

parse_err_t parse_symbol(parse_stream_t *stream, ast_node_t *ret)
{
  sv_t symbol = sv_chop_left(stream->contents, stream->byte);
  symbol.size = sv_while(symbol, SYMBOL_CHARS);

  // see if symbol is one of the already known symbols
  static_assert(NUM_AST_KNOWNS == 2, "Expected number of AST KNOWN");
  for (ast_known_t i = 0; i < NUM_AST_KNOWNS; ++i)
  {
    const char *possible_known = ast_known_to_cstr(i);
    if (strlen(possible_known) == symbol.size &&
        strncmp(possible_known, symbol.data, symbol.size) == 0)
    {
      // Found a matching known symbol
      *ret = ast_node_known(stream->byte, i);
      goto end;
    }
  }

  // otherwise, it must be a fresh symbol i.e. user defined
  *ret = ast_node_symbol(stream->byte, symbol);
end:
  stream_advance(stream, symbol.size);
  return PARSE_ERR_OK;
}

bool stream_eos(parse_stream_t *stream)
{
  return stream->byte >= stream->contents.size;
}

char stream_peek(parse_stream_t *stream)
{
  if (stream_eos(stream))
    return '\0';
  else
    return stream->contents.data[stream->byte];
}

void stream_advance(parse_stream_t *stream, u64 size)
{
  if (stream->byte + size >= stream->contents.size)
    stream->byte = stream->contents.size;
  else
    stream->byte += size;
}

u64 stream_size(parse_stream_t *stream)
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
