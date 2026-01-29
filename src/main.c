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

int read_file(const char *filename, sv_t *ret)
{
  // NOTE: Stupidly simple.  Presumes the file is NOT three pipes in a trench
  // coat.
  FILE *fp = fopen(filename, "rb");
  if (!fp)
    return 1;

  fseek(fp, 0, SEEK_END);
  ret->size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  ret->data = calloc(1, ret->size + 1);
  fread(ret->data, ret->size, 1, fp);
  fclose(fp);

  ret->data[ret->size] = '\0';
  return 0;
}

int read_pipe(FILE *pipe, sv_t *ret)
{
  // NOTE: We can't read an entire pipe at once like we did for read_file.  So
  // let's read in buffered chunks, with a vector to keep them contiguous.
  vec_t contents = {0};
  char buffer[1024];
  while (!feof(pipe))
  {
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), pipe);
    vec_append(&contents, buffer, bytes_read);
  }

  ret->size = contents.size;
  // Get that null terminator in, but only after we've recorded the actual size
  // of what's been read.
  vec_append_byte(&contents, '\0');

  if (contents.not_inlined)
  {
    // Take the heap pointer from us.
    ret->data = vec_data(&contents);
  }
  else
  {
    // vec_data(&contents) is stack allocated; can't carry that out of this
    // function!
    ret->data = calloc(1, contents.size);
    memcpy(ret->data, vec_data(&contents), contents.size);
  }
  return 0;
}

void usage(FILE *fp)
{
  fprintf(fp, "Usage: arl [FILE]\n"
              "Compiles [FILE] as ARL source code.\n"
              "  [FILE]: File to compile.\n"
              "If FILE is \"--\", then read from stdin.\n");
}

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

  LOG("Lexed %lu tokens\n", tokens.vec.size / sizeof(token_t));
#if VERBOSE_LOGS
  token_stream_print(stdout, &tokens);
#endif
  printf("\n");

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
