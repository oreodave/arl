/* ast.c: Implementation of AST constructor/destructor functions
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See ast.h.
 */

#include <arl/parser/ast.h>

obj_t obj_string(u64 line, u64 col, sv_t string)
{
  return (obj_t){
      .line   = line,
      .column = col,
      .type   = TYPE_STRING,
      .value  = {string},
  };
}

obj_t obj_symbol(u64 line, u64 col, sv_t symbol)
{
  return (obj_t){
      .line   = line,
      .column = col,
      .type   = TYPE_SYMBOL,
      .value  = {symbol},
  };
}

void ast_free(ast_t *ast)
{
  // we can free the vector itself and we're done
  vec_free(&ast->objects);
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
