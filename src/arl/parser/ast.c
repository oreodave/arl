/* ast.c: Implementation of AST constructor/destructor functions
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See ast.h.
 */

#include <arl/lib/base.h>
#include <arl/lib/vec.h>
#include <arl/parser/ast.h>

const char *ast_prim_to_cstr(ast_prim_t prim)
{
  switch (prim)
  {
  case AST_PRIM_NIL:
    return "nil";
  case AST_PRIM_PRINTLN:
    return "println";
  default:
    FAIL("Unexpected AST primitive value: %d\n", prim);
  }
}

ast_node_t ast_node_prim(u64 byte, ast_prim_t primitive)
{
  return (ast_node_t){
      .byte_location = byte,
      .type          = AST_NODE_TYPE_PRIMITIVE,
      .value         = {.as_prim = primitive},
  };
}

ast_node_t ast_node_string(u64 byte, sv_t string)
{
  return (ast_node_t){
      .byte_location = byte,
      .type          = AST_NODE_TYPE_STRING,
      .value         = {.as_string = string},
  };
}

ast_node_t ast_node_symbol(u64 byte, sv_t symbol)
{
  return (ast_node_t){
      .byte_location = byte,
      .type          = AST_NODE_TYPE_SYMBOL,
      .value         = {.as_symbol = symbol},
  };
}

void ast_node_print(FILE *fp, ast_node_t *node)
{
  if (!node)
  {
    fprintf(fp, "NIL");
    return;
  }
  switch (node->type)
  {
  case AST_NODE_TYPE_PRIMITIVE:
    fprintf(fp, "PRIMITIVE(%s)", ast_prim_to_cstr(node->value.as_prim));
    break;
  case AST_NODE_TYPE_SYMBOL:
    fprintf(fp, "SYMBOL(" PR_SV ")", SV_FMT(node->value.as_symbol));
    break;
  case AST_NODE_TYPE_STRING:
    fprintf(fp, "STRING(" PR_SV ")", SV_FMT(node->value.as_string));
    break;
  case NUM_AST_NODE_TYPES:
  default:
    FAIL("Unexpected node type: %d\n", node->type);
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
