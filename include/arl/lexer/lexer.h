/* lexer.h: Lexer which takes character buffers and yields a sequence of tokens.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef LEXER_H
#define LEXER_H

#include <arl/lexer/token.h>

/// Token streams, utilised when lexing.
typedef struct
{
  u64 byte;
  sv_t contents;
} lex_stream_t;

/// Types of errors that may occur during lexing
typedef enum
{
  LEX_ERR_OK = 0,
  LEX_ERR_EXPECTED_SPEECH_MARKS,
  LEX_ERR_UNKNOWN_CHAR,
} lex_err_t;
const char *lex_err_to_string(lex_err_t err);

// Generates a token stream from a lex_stream_t, storing it in OUT.  Returns any
// errors it may generate.
lex_err_t lex_stream(token_stream_t *out, lex_stream_t *stream);

// Computes the line and column that STREAM is currently pointing at in its
// buffer, storing it in LINE and COL.
void lex_stream_get_line_col(lex_stream_t *stream, u64 *line, u64 *col);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
