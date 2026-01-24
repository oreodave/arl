/* parser.h: Parser which takes character buffers and yields an AST
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef PARSER_H
#define PARSER_H

#include <arl/parser/ast.h>

/// Parser streams, utilised when generating an AST.
typedef struct
{
  u64 byte;
  sv_t contents;
} parse_stream_t;

/// Types of errors that may occur during parsing
typedef enum
{
  PARSE_ERR_OK = 0,
  PARSE_ERR_EXPECTED_SPEECH_MARKS,
  PARSE_ERR_UNKNOWN_CHAR,
} parse_err_t;
const char *parse_err_to_string(parse_err_t err);

// Generates an AST from STREAM, storing it in OUT.  Returns any errors it may
// generate.
parse_err_t parse(ast_t *out, parse_stream_t *stream);

// Computes the line and column that STREAM is currently pointing at in its
// buffer, storing it in LINE and COL.
void parse_stream_get_line_col(parse_stream_t *stream, u64 *line, u64 *col);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
