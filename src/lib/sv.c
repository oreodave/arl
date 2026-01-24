/* sv.c: String View implementations
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary: See /include/arl/lib/sv.h
 */

#include <string.h>

#include <arl/lib/sv.h>

sv_t sv_chop_left(sv_t sv, u64 len)
{
  if (sv.size < len)
  {
    return SV(NULL, 0);
  }
  else
  {
    return SV(sv.data + len, sv.size - len);
  }
}

sv_t sv_chop_right(sv_t sv, u64 len)
{
  if (sv.size < len)
  {
    return SV(NULL, 0);
  }
  else
  {
    return SV(sv.data, sv.size - len);
  }
}

u64 sv_while(const sv_t sv, const char *expected)
{
  u64 i;
  for (i = 0; i < sv.size && strchr(expected, sv.data[i]) != NULL; ++i)
    continue;
  return i;
}

u64 sv_till(const sv_t sv, const char *expected)
{
  u64 i;
  for (i = 0; i < sv.size && strchr(expected, sv.data[i]) == NULL; ++i)
    continue;
  return i;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the MIT License
 * for details.

 * You may distribute and modify this code under the terms of the
 * MIT License, which you should have received a copy of along with this
 * program.  If not, please go to <https://opensource.org/license/MIT>.

 */
