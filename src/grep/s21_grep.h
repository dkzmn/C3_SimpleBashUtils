#ifndef SRC_GREP_MAIN_H_
#define SRC_GREP_MAIN_H_

#include <getopt.h>
#include <locale.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int invalid, e_flag, i_flag, v_flag, c_flag, l_flag, n_flag, h_flag, s_flag,
      f_flag, o_flag;
} Params;

typedef struct {
  int position, end, filecount, lineinfile;
} Fp;

int get_params(Params *p, int argc, char **argv, int *pcount, char ***patterns,
               int *fcount, char ***files);
int go(Params p, int fcount, char **files, int pcount, char **patterns);
int add_str(char *str, int *arr_count, char ***array);
int add_found(Fp f, int *arr_count, Fp **array);
int sort(int count, Fp *array);
int print_str(Params p, char *str, int count, int fcount, char *fn, int nline);

#endif  // SRC_GREP_MAIN_H_
