/* main.cpp:
 * Created: 2026-01-22
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define FAIL(...)                 \
  do                              \
  {                               \
    fprintf(stderr, "FAIL: ");    \
    fprintf(stderr, __VA_ARGS__); \
    abort();                      \
  } while (0)

char *read_file(const char *filename)
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
  return buffer;
}

int main(void)
{
  const char *filename = "./examples/hello-world.arl";
  char *buffer         = read_file(filename);
  printf("%s => %s\n", filename, buffer);
  free(buffer);
  return 0;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the MIT License for details.

 * You may distribute and modify this code under the terms of the MIT License,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://opensource.org/license/MIT>.

 */
