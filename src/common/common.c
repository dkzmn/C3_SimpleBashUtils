#include "common.h"

int read_str(char *str, FILE *fp) {
  int i = 0;
  while (1) {
    str[i] = fgetc(fp);
    if (feof(fp) != 0) break;
    i++;
    if (str[i - 1] == '\n') break;
  }
  str[i] = '\0';
  if (i == 0) i = -1;
  // printf("(%d)%s!\n", i, str);
  return i;
}
