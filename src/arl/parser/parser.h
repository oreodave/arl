/* parser.h: Parser which takes character buffers and yields an AST
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef PARSER_H
#define PARSER_H

#include <arl/parser/ast.h>

typedef enum
{
  PARSE_ERR_OK = 0,
  PARSE_ERR_UNEXPECTED_EOF,
  PARSE_ERR_EXPECTED_SPEECH_MARKS,
  PARSE_ERR_UNKNOWN_CHAR,
} parse_err_t;
const char *parse_err_to_string(parse_err_t err);

typedef struct
{
  u64 line, column, cursor;
  sv_t contents;
} parse_stream_t;

parse_err_t parse(ast_t *out, parse_stream_t *stream);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
