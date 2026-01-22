/* ast.h: General definition of the AST and objects within it.
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
  TYPE_SYMBOL,
  TYPE_STRING,
} type_t;

/// Node of the AST as a tagged union
typedef struct
{
  u64 byte;
  type_t type;
  union
  {
    sv_t as_string;
    sv_t as_symbol;
  } value;
} obj_t;

// Constructors
obj_t obj_string(u64 byte, sv_t string);
obj_t obj_symbol(u64 byte, sv_t symbol);

void obj_print(FILE *fp, obj_t *obj);

/// The AST as a flat collection of nodes
typedef struct
{
  vec_t objects;
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
