/* ast.h: General definition of the AST and nodes within it.
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef AST_H
#define AST_H

#include <arl/lib/base.h>
#include <arl/lib/sv.h>
#include <arl/lib/vec.h>

/// Types the AST can encode
typedef enum
{
  AST_NODE_TYPE_PRIMITIVE = 0,
  AST_NODE_TYPE_SYMBOL,
  AST_NODE_TYPE_STRING,

  NUM_AST_NODE_TYPES,
} ast_node_type_t;

/// Primitives (values, callables, etc) as symbols
typedef enum
{
  AST_PRIM_NIL = 0,
  AST_PRIM_PUTSTR,

  NUM_AST_PRIMS,
} ast_prim_t;

const char *ast_prim_to_cstr(ast_prim_t);

/// Node of the AST as a tagged union
typedef struct
{
  u64 byte_location;
  ast_node_type_t type;
  union
  {
    ast_prim_t as_prim;
    sv_t as_symbol;
    sv_t as_string;
  } value;
} ast_node_t;

ast_node_t ast_node_prim(u64 byte, ast_prim_t primitive);
ast_node_t ast_node_symbol(u64 byte, sv_t symbol);
ast_node_t ast_node_string(u64 byte, sv_t string);
void ast_node_print(FILE *fp, ast_node_t *node);

/// The AST as a flat collection of nodes
typedef struct
{
  vec_t nodes;
} ast_t;

void ast_free(ast_t *ast);
void ast_print(FILE *fp, ast_t *ast);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
