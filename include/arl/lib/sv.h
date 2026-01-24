/* sv.h: String Views
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: Absolute bajoding
 */

#ifndef SV_H
#define SV_H

#include <arl/lib/base.h>

typedef struct
{
  char *data;
  u64 size;
} sv_t;

// Constructor and printer macros
#define SV(PTR, SIZE) ((sv_t){.data = (PTR), .size = (SIZE)})
#define PR_SV         "%.*s"
#define SV_FMT(SV)    (int)((SV).size), (SV).data

// Return a new string view which has had LEN characters removed from the
// beginning
sv_t sv_chop_left(sv_t sv, u64 len);
// Return a new string view which has had LEN characters removed from the end
sv_t sv_chop_right(sv_t sv, u64 len);
// Return the first index where SV does not present a character from EXPECTED
// (strspn equivalent)
u64 sv_while(const sv_t sv, const char *expected);
// Return the first index where SV presents a character from EXPECTED (strcspn
// equivalent)
u64 sv_till(const sv_t sv, const char *expected);

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
