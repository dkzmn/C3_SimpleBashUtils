#include "s21_grep.h"

#include "../common/common.h"

int main(int argc, char *argv[]) {
  Params p;
  char **files = NULL, **patterns = NULL;
  int fcount = 0, pcount = 0;
  get_params(&p, argc, argv, &pcount, &patterns, &fcount, &files);
  if (p.invalid == 0) go(p, fcount, files, pcount, patterns);
  for (int i = 0; i < fcount; i++) free(files[i]);
  free(files);
  for (int i = 0; i < pcount; i++) free(patterns[i]);
  free(patterns);
  return 0;
}

int get_params(Params *p, int argc, char **argv, int *pcount, char ***patterns,
               int *fcount, char ***files) {
  p->invalid = 0;
  p->e_flag = 0;
  p->i_flag = 0;
  p->v_flag = 0;
  p->c_flag = 0;
  p->l_flag = 0;
  p->n_flag = 0;
  p->h_flag = 0;
  p->s_flag = 0;
  p->f_flag = 0;
  p->o_flag = 0;

  char short_opt[] = "e:ivclnhsf:o";

  static struct option long_opt[] = {{NULL, 0, NULL, 0}};

  int c;
  char **pfiles = NULL;
  int pfcount = 0;
  while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != EOF) {
    switch (c) {
      case 'e':
        p->e_flag = 1;
        add_str(optarg, pcount, patterns);
        break;
      case 'i':
        p->i_flag = 1;
        break;
      case 'v':
        p->v_flag = 1;
        break;
      case 'c':
        p->c_flag = 1;
        break;
      case 'l':
        p->l_flag = 1;
        break;
      case 'n':
        p->n_flag = 1;
        break;
      case 'h':
        p->h_flag = 1;
        break;
      case 's':
        p->s_flag = 1;
        break;
      case 'f':
        p->f_flag = 1;
        add_str(optarg, &pfcount, &pfiles);
        break;
      case 'o':
        p->o_flag = 1;
        break;
      case '?':
        p->invalid++;
    }
  }

  for (int i = 0; i < pfcount; i++) {
    FILE *fp = fopen(pfiles[i], "r");
    if (fp == NULL) {
      if (p->s_flag == 0) {
        printf("grep: %s: No such file or directory\n", optarg);
      }
    } else {
      int bufsize = 4096;
      char buffer[bufsize];
      int count;
      while ((count = read_str(buffer, fp)) != -1) {
        if (buffer[count - 1] == '\n') buffer[count - 1] = 0;
        add_str(buffer, pcount, patterns);
      }
    }
  }
  if ((*pcount == 0) && (optind < argc)) {
    add_str(argv[optind], pcount, patterns);
    optind++;
  }
  if (optind < argc) {
    *fcount = argc - optind;
    *files = malloc((argc - optind) * sizeof(char *));
    int i = 0;
    do {
      (*files)[i] = malloc((strlen(argv[optind]) + 1) * sizeof(char));
      strcpy((*files)[i], argv[optind]);
      i++;
    } while (++optind < argc);
  } else {
    *fcount = 0;
  }
  for (int i = 0; i < pfcount; i++) free(pfiles[i]);
  free(pfiles);
  return 0;
}

int go(Params p, int fcount, char **files, int pcount, char **patterns) {
  FILE *fp;
  int bufsize = 4096;
  char buffer[bufsize];
  int isApple = 0;
#ifdef __APPLE__
  isApple = 1;
#endif
  for (int i = 0; i < fcount; i++) {
    fp = fopen(files[i], "r");
    if (fp == NULL) {
      if (p.s_flag == 0) {
        printf("grep: %s: No such file or directory\n", files[i]);
      }
    } else {
      int count;
      int lif = 0, count_all = 0, count_lines = 0;
      while ((count = read_str(buffer, fp)) != -1) {
        int found = 0, found_o = 0;
        lif++;
        Fp *found_patterns = NULL;
        char *ps = buffer;
        int position = 0;
        for (int j = 0; j < pcount; j++) {
          regex_t re;
          char buf[256];
          regmatch_t pmatch[100];
          int status;
          int eflag;
          setlocale(LC_ALL, "");
          int regcomp_flag = REG_NEWLINE;
          if (p.i_flag == 1) regcomp_flag |= REG_ICASE;
          int len2 = strlen(patterns[j]);
          if (len2 == 0) {
            found++;
          } else {
            if ((status = regcomp(&re, patterns[j], regcomp_flag)) != 0) {
              regerror(status, &re, buf, 255);
            } else {
              if (isApple == 0) {
                ps = buffer;
                position = 0;
              }
              eflag = 0;
              while ((status = regexec(&re, ps, 1, pmatch, eflag)) == 0) {
                found++;
                count_all++;
                if ((p.o_flag == 1) && (p.v_flag == 0) && (p.c_flag == 0) &&
                    (p.l_flag == 0)) {
                  Fp f;
                  f.position = position + pmatch[0].rm_so;
                  f.end = pmatch[0].rm_eo - pmatch[0].rm_so;
                  f.filecount = fcount;
                  f.lineinfile = lif;
                  add_found(f, &found_o, &found_patterns);
                }
                ps += pmatch[0].rm_eo;
                position += pmatch[0].rm_eo;
                eflag = REG_NOTBOL;
              }
            }
          }
          regfree(&re);
        }
        sort(found_o, found_patterns);
        for (int ii = 0; ii < found_o; ii++) {
          int nfc = found_patterns[ii].filecount;
          int nlif = found_patterns[ii].lineinfile;
          if (ii > 0) {
            int nlif1 = found_patterns[ii - 1].lineinfile;
            if ((nlif == nlif1) && (isApple == 1)) {
              nfc = 0;
              nlif = 0;
            }
          }
          print_str(p, buffer + found_patterns[ii].position,
                    found_patterns[ii].end, nfc, files[i], nlif);
        }
        free(found_patterns);
        if (found > 0) count_lines++;
        int cond = 0;
        if (isApple == 1)
          cond = ((p.v_flag == 1) && (p.o_flag == 1) && (p.l_flag == 0) &&
                  (p.c_flag == 0));
        if (((p.c_flag == 0) && (p.l_flag == 0) && (p.o_flag == 0)) || cond) {
          if (p.v_flag == 0) {
            if (found > 0) print_str(p, buffer, count, fcount, files[i], lif);
          } else {
            if (found == 0) print_str(p, buffer, count, fcount, files[i], lif);
          }
        }
      }
      int v_count_lines = lif - count_lines;
      int compare_cl;
      if (p.v_flag == 1)
        compare_cl = lif - count_lines;
      else
        compare_cl = count_lines;
      if ((p.l_flag == 1) && (compare_cl > 0) && (isApple == 1)) {
        count_lines = 1;
        v_count_lines = 1;
      }
      int cond2 = 1;
      if (isApple == 0) cond2 = (p.l_flag == 0);
      if ((p.c_flag == 1) && cond2) {
        char strcount[10];
        if (p.v_flag == 0)
          sprintf(strcount, "%d", count_lines);
        else
          sprintf(strcount, "%d", v_count_lines);
        print_str(p, strcount, strlen(strcount), fcount, files[i], 0);
      }
      if ((p.l_flag == 1) && ((p.v_flag == 1) || (count_all > 0)))
        printf("%s\n", files[i]);
      fclose(fp);
    }
  }
  return 0;
}

int add_str(char *str, int *arr_count, char ***array) {
  int r = 0;
  void *temp = realloc(*array, (*arr_count + 1) * sizeof(char *));
  if (temp != NULL) {
    *array = temp;
    (*array)[(*arr_count)] = malloc((strlen(str) + 1) * sizeof(char));
    strcpy((*array)[(*arr_count)], str);
    (*arr_count)++;
  } else {
    r = -1;
  }
  return r;
}

int add_found(Fp f, int *arr_count, Fp **array) {
  int r = 0;
  void *temp = realloc(*array, (*arr_count + 1) * sizeof(Fp));
  if (temp != NULL) {
    *array = temp;
    (*array)[(*arr_count)] = f;
    (*arr_count)++;
  } else {
    r = -1;
  }
  return r;
}

int sort(int count, Fp *array) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (array[j].position > array[j + 1].position) {
        Fp tmp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = tmp;
      }
    }
  }
  return 0;
}

int print_str(Params p, char *str, int count, int fcount, char *fn, int nline) {
  if ((fcount > 1) && (p.h_flag == 0)) printf("%s:", fn);
  if ((p.n_flag == 1) && (nline > 0)) printf("%d:", nline);
  for (int i = 0; i < count; i++) {
    fputc(str[i], stdout);
  }
  if (str[count - 1] != '\n') fputc('\n', stdout);
  return 0;
}
