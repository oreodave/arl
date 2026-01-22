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

typedef enum
{
  TYPE_SYMBOL,
  TYPE_STRING,
} type_t;

typedef struct
{
  u64 line, column;

  type_t type;
  union
  {
    sv_t as_string;
    sv_t as_symbol;
  } value;
} obj_t;

obj_t obj_string(u64 line, u64 col, sv_t string);
obj_t obj_symbol(u64 line, u64 col, sv_t symbol);

// Our AST is simply a vector of objects.  Nesting and tree like structure is
// imposed by individual objects.
typedef struct
{
  vec_t objects;
} ast_t;

void ast_free(ast_t *ast);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
