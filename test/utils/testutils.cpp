//
// Created by kiran on 5/20/24.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>

const char *insertIntoTempFile(const char *contents) {
  char *tempFile = strdup("/tmp/kl-XXXXXX");
  mkstemp(tempFile);
  FILE *file = fopen(tempFile, "w");
  fprintf(file, "%s", contents);
  fclose(file);
  return tempFile;
}