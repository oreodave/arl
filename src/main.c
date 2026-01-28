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

#include <arl/lib/base.h>
#include <arl/lib/sv.h>
#include <arl/lib/vec.h>
#include <arl/parser/ast.h>
#include <arl/parser/parser.h>

int read_file(const char *filename, sv_t *ret)
{
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

int main(void)
{
  int ret              = 0;
  const char *filename = "./examples/hello-world.arl";
  sv_t contents        = {0};
  if (read_file(filename, &contents))
  {
    LOG_ERR("ERROR: Reading `%s`: ", filename);
    perror("");
    ret = 1;
    goto end;
  }

  LOG("%s => `" PR_SV "`\n", filename, SV_FMT(contents));

  parse_stream_t stream = {.byte = 0, .contents = contents};
  ast_t ast             = {0};
  parse_err_t perr      = parse(&ast, &stream);
  if (perr)
  {
    u64 line = 1, col = 0;
    parse_stream_get_line_col(&stream, &line, &col);

    LOG_ERR("%s:%lu:%lu: %s\n", filename, line, col, parse_err_to_string(perr));
    ret = 1;
    goto end;
  }

  LOG("Parsed %lu nodes\n", ast.nodes.size / sizeof(ast_node_t));
#if VERBOSE_LOGS
  ast_print(stdout, &ast);
#endif
  printf("\n");

end:
  if (contents.data)
    free(contents.data);
  if (ast.nodes.capacity > 0)
    ast_free(&ast);
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
