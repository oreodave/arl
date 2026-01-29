/* main.c:
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arl/lexer/lexer.h>
#include <arl/lexer/token.h>
#include <arl/lib/base.h>
#include <arl/lib/sv.h>
#include <arl/lib/vec.h>

#include <arl/cli.h>

int main(int argc, char *argv[])
{
  int ret        = 0;
  char *filename = "";
  if (argc == 1)
  {
    usage(stderr);
    ret = 1;
    goto end;
  }
  else
  {
    filename = argv[1];
  }

  int read_err  = 0;
  sv_t contents = {0};
  if (strcmp(filename, "--") == 0)
  {
    filename = "stdin";
    read_err = read_pipe(stdin, &contents);
  }
  else
  {
    read_err = read_file(filename, &contents);
  }

  if (read_err)
  {
    LOG_ERR("ERROR: Reading `%s`: ", filename);
    perror("");
    ret = 1;
    goto end;
  }

  LOG("%s => `" PR_SV "`\n", filename, SV_FMT(contents));

  lex_stream_t stream   = {.byte = 0, .contents = contents};
  token_stream_t tokens = {0};
  lex_err_t perr        = lex_stream(&tokens, &stream);
  if (perr)
  {
    u64 line = 1, col = 0;
    lex_stream_get_line_col(&stream, &line, &col);

    LOG_ERR("%s:%lu:%lu: %s\n", filename, line, col, lex_err_to_string(perr));
    ret = 1;
    goto end;
  }

#if VERBOSE_LOGS
  LOG("Lexed %lu tokens ", tokens.vec.size / sizeof(token_t));
  token_stream_print(stdout, &tokens);
  printf("\n");
#endif

end:
  if (contents.data)
    free(contents.data);
  token_stream_free(&tokens);
  return ret;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
