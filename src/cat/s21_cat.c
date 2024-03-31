#include "s21_cat.h"

#include "../common/common.h"

int main(int argc, char *argv[]) {
  Params p;
  char **files;
  int fcount = 0;
  get_params(&p, argc, argv, &fcount, &files);
  if (p.invalid == 0) go(p, fcount, files);
  for (int i = 0; i < fcount; i++) free(files[i]);
  free(files);
  return 0;
}

int get_params(Params *p, int argc, char **argv, int *fcount, char ***files) {
  p->invalid = 0;
  p->b_flag = 0;
  p->e_flag = 0;
  p->n_flag = 0;
  p->s_flag = 0;
  p->t_flag = 0;
  p->v_flag = 0;
  char short_opt[] = "beEnstTv";

  static struct option long_opt[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  int c;
  while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != EOF) {
    switch (c) {
      case 'b':
        p->b_flag = 1;
        break;
      case 'e':
        p->e_flag = 1;
        p->v_flag = 1;
        break;
      case 'E':
        p->e_flag = 1;
        break;
      case 'n':
        p->n_flag = 1;
        break;
      case 's':
        p->s_flag = 1;
        break;
      case 't':
        p->t_flag = 1;
        p->v_flag = 1;
        break;
      case 'T':
        p->t_flag = 1;
        break;
      case 'v':
        p->v_flag = 1;
        break;
      case '?':
        p->invalid++;
    }
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
  return 0;
}

int go(Params p, int fcount, char **files) {
  FILE *fp;
  int bufsize = 4096;
  char buffer[bufsize];
  int line = 0, lastempty = 0, lastendsnewstr = 1;

  for (int i = 0; i < fcount; i++) {
    fp = fopen(files[i], "r");
    if (fp == NULL) {
      printf("cat: %s: No such file or directory\n", files[i]);
    } else {
      int count;
      while ((count = read_str(buffer, fp)) != -1) {
        int empty = 0;
        if (strcmp(buffer, "\n") == 0) empty = 1;
        if (p.t_flag == 1) {
          for (int i = 0; i < count; i++) {
            if (buffer[i] == '\t') {
              for (int j = count + 1; j > i + 1; j--) buffer[j] = buffer[j - 1];
              buffer[i] = '^';
              buffer[i + 1] = 'I';
              count++;
            }
          }
        }
        if ((p.e_flag == 1) && (buffer[count - 1] == '\n')) {
          buffer[count - 1] = '$';
          buffer[count] = '\n';
          buffer[count + 1] = 0;
          count++;
        }
        if (p.v_flag == 1) {
          for (int i = 0; i < count; i++) {
            if ((buffer[i] >= 0) && (buffer[i] < 32) && (buffer[i] != 9) &&
                (buffer[i] != 10)) {
              for (int j = count + 1; j > i + 1; j--) buffer[j] = buffer[j - 1];
              buffer[i + 1] = buffer[i] + 64;
              buffer[i] = '^';
              count++;
            }
            if (buffer[i] == 127) {
              for (int j = count + 1; j > i + 1; j--) buffer[j] = buffer[j - 1];
              buffer[i + 1] = '?';
              buffer[i] = '^';
              count++;
            }
            if (((unsigned char)buffer[i] >= 128) &&
                ((unsigned char)buffer[i] < 160)) {
              for (int j = count + 3; j > i + 1; j--) buffer[j] = buffer[j - 3];
              buffer[i + 3] = buffer[i] - 64;
              buffer[i + 2] = '^';
              buffer[i + 1] = '-';
              buffer[i] = 'M';
              count = count + 3;
            }
#ifndef __APPLE__
            if ((unsigned char)buffer[i] >= 160) {
              for (int j = count + 2; j > i + 1; j--) buffer[j] = buffer[j - 2];
              buffer[i + 2] = buffer[i] - 128;
              buffer[i + 1] = '-';
              buffer[i] = 'M';
              count = count + 2;
            }
#endif
          }
        }
        print_str(p, &lastempty, &line, &lastendsnewstr, buffer, count, empty);
      }
      fclose(fp);
#ifdef __APPLE__
      line = 0;
#endif
    }
  }
  return 0;
}

int print_str(Params p, int *lastempty, int *line, int *lastendsnewstr,
              char *str, int count, int empty) {
  if (!((p.s_flag == 1) && (*lastempty == 1) && (empty == 1))) {
#ifndef __APPLE__
    if (*lastendsnewstr == 1) {
#endif
      if (((p.n_flag == 1) && (p.b_flag == 0)) ||
          ((p.b_flag == 1) && (empty == 0))) {
        (*line)++;
        printf("%6d\t", *line);
      }
#ifndef __APPLE__
    }
#endif
    for (int i = 0; i < count; i++) {
      fputc(str[i], stdout);
    }

    if (str[count - 1] == '\n') {
      *lastendsnewstr = 1;
    } else {
      *lastendsnewstr = 0;
    }
    if (empty == 1) {
      *lastempty = 1;
    } else {
      *lastempty = 0;
    }
  }
  return 0;
}
