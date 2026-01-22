/* main.cpp:
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

#include <arl/lib/base.h>
#include <arl/lib/sv.h>
#include <arl/lib/vec.h>
#include <arl/parser/parser.h>

/// Parser
sv_t read_file(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (!fp)
    FAIL("File `%s` does not exist\n", filename);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = calloc(1, size + 1);
  fread(buffer, size, 1, fp);
  fclose(fp);

  buffer[size] = '\0';
  return SV(buffer, size);
}

int main(void)
{
  const char *filename = "./examples/hello-world.arl";
  sv_t contents        = read_file(filename);
  printf("%s => " PR_SV "\n", filename, SV_FMT(contents));

  parse_stream_t stream = {.line = 1, .column = 0, .contents = contents};
  ast_t ast             = {0};
  parse_err_t perr      = parse(&ast, &stream);
  if (perr)
  {
    fprintf(stderr, "%s:%lu:%lu: %s\n", filename, stream.line, stream.column,
            parse_err_to_string(perr));
    goto fail;
  }

  free(contents.data);
  ast_free(&ast);
  return 0;
fail:
  if (contents.data)
    free(contents.data);
  if (ast.objects.capacity > 0)
    ast_free(&ast);
  return 1;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
