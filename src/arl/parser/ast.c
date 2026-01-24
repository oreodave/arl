/* ast.c: Implementation of AST constructor/destructor functions
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See ast.h.
 */

#include <arl/lib/vec.h>
#include <arl/parser/ast.h>

ast_node_t ast_node_string(u64 byte, sv_t string)
{
  return (ast_node_t){
      .byte_location = byte,
      .type          = AST_NODE_TYPE_STRING,
      .value         = {string},
  };
}

ast_node_t ast_node_symbol(u64 byte, sv_t symbol)
{
  return (ast_node_t){
      .byte_location = byte,
      .type          = AST_NODE_TYPE_SYMBOL,
      .value         = {symbol},
  };
}

void ast_node_print(FILE *fp, ast_node_t *obj)
{
  if (!obj)
  {
    fprintf(fp, "NIL");
    return;
  }
  switch (obj->type)
  {
  case AST_NODE_TYPE_SYMBOL:
    fprintf(fp, "SYMBOL(" PR_SV ")", SV_FMT(obj->value.as_symbol));
    break;
  case AST_NODE_TYPE_STRING:
    fprintf(fp, "STRING(" PR_SV ")", SV_FMT(obj->value.as_string));
    break;
  }
}

void ast_print(FILE *fp, ast_t *ast)
{
  if (!ast)
  {
    fprintf(fp, "{}");
    return;
  }
  fprintf(fp, "{");
  if (ast->nodes.size == 0)
  {
    fprintf(fp, "}\n");
    return;
  }

  fprintf(fp, "\n");
  for (u64 i = 0; i < ast->nodes.size / sizeof(ast_node_t); ++i)
  {
    ast_node_t item = VEC_GET(&ast->nodes, i, ast_node_t);
    fprintf(fp, "\t[%lu]: ", i);
    ast_node_print(fp, &item);
    fprintf(fp, "\n");
  }
  fprintf(fp, "}");
}

void ast_free(ast_t *ast)
{
  // we can free the vector itself and we're done
  vec_free(&ast->nodes);
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
