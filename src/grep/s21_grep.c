#define _GNU_SOURCE
#include "s21_grep.h"

int main(int argc, char *argv[]) {
  options flag = {0};
  options *flags = &flag;
  char pattern[SIZE];

  finder_flags(argc, argv, pattern, flags);
  print_file_flag(flags, optind, argc);
  error(flags, argc);
  files_output_2(argc, argv, flags, optind, pattern);

  return 0;
}

void finder_flags(int argc, char **argv, char *pattern, options *flags) {
  int checkFlag = 0;
  if (argc == 1) {
    flags->flag_error = 1;
  } else {
    while ((checkFlag = getopt(argc, argv, "e:ivclnhsof:")) != -1) {
      switch (checkFlag) {
        case 'e':
          flags->flag_e = 1;
          if (strlen(pattern) > 0) {
            strcat(pattern, "|");
          }
          strcat(pattern, optarg);
          break;
        case 'i':
          flags->flag_i = 1;
          break;
        case 'v':
          flags->flag_v = 1;
          break;
        case 'c':
          flags->flag_c = 1;
          break;
        case 'l':
          flags->flag_l = 1;
          break;
        case 'n':
          flags->flag_n = 1;
          break;
        case 'h':
          flags->flag_h = 1;
          break;
        case 's':
          flags->flag_s = 1;
          opterr = 0;
          break;
        case 'o':
          flags->flag_o = 1;
          break;
        case 'f':
          flags->flag_f = 1;
          FILE *file;
          file = fopen(optarg, "r");
          char symbol = fgetc(file);
          for (int i = 0; symbol != EOF; i++) {
            if (symbol == '\n') {
              symbol = '|';
            }
            pattern[i] = symbol;
            symbol = fgetc(file);
          }
          fclose(file);
          break;
        default:
          flags->flag_error = 1;
          break;
      }
    }
  }
}

void print_file_flag(options *flags, int optind, int argc) {
  if (flags->flag_e == 0 || flags->flag_f == 0) {
    if (optind + 2 < argc) {
      flags->print_file_flag = 1;
    }
  } else if (flags->flag_e || flags->flag_f) {
    if (optind + 1 < argc) {
      flags->print_file_flag = 1;
    }
  }
}

void files_output_2(int argc, char **argv, options *flags, int optind,
                    char *pattern) {
  if (flags->flag_e || flags->flag_f) {
    for (int i = optind; i < argc; i++) {
      print_grep_2(argv[i], pattern, argv, flags);
    }
  } else {
    for (int i = optind + 1; i < argc; i++) {
      print_grep_2(argv[i], pattern, argv, flags);
    }
  }
}

void print_grep_2(char *file_name, char *pattern, char **argv, options *flags) {
  FILE *fp;
  char *line_in_file = NULL;
  size_t len = 0;
  ssize_t read;
  int cnt_line = 0, cnt_coincidence = 0, res_search;
  regex_t reegex;
  regmatch_t p_match[1];
  char str[100] = {0};

  if (flags->flag_e == 0 && flags->flag_f == 0) {
    regcomp(&reegex, argv[optind], REG_EXTENDED);
  } else {
    regcomp(&reegex, pattern, REG_EXTENDED);
  }
  if (((fp = fopen(file_name, "r")) == NULL)) {
    if (flags->flag_s == 0) {
      printf("grep: %s No such file or directory\n", file_name);
    }
  } else {
    while ((read = getline(&line_in_file, &len, fp)) != -1) {
      cnt_line++;
      res_search = regexec(&reegex, line_in_file, 0, NULL, 0);

      if (flags->flag_i && flags->flag_e) {
        regfree(&reegex);
        regcomp(&reegex, pattern, REG_ICASE);
        res_search = regexec(&reegex, line_in_file, 0, NULL, 0);
      }
      if ((flags->flag_i && !flags->flag_e) ||
          (flags->flag_i && flags->flag_o)) {
        regfree(&reegex);
        regcomp(&reegex, argv[optind], REG_ICASE);
        res_search = regexec(&reegex, line_in_file, (size_t)1, p_match, 0);
        if (!(res_search)) {
          for (int i = p_match->rm_so; i < p_match->rm_eo; i++) {
            str[i - p_match->rm_so] = (line_in_file)[i];
          }
        }
      }
      if (flags->flag_v) {
        res_search = !res_search;
      }
      if (flags->flag_h == 1) {
        flags->print_file_flag = 0;
      }
      if (res_search == 0) {
        cnt_coincidence++;
        if (flags->flag_l && cnt_line != 0 && res_search == 0) {
          cnt_line = 1;
          printf("%s\n", file_name);
          break;
        }
        s21_grep_logik(flags, line_in_file, file_name, cnt_line, argv, pattern,
                       str);
      }
    }
    print_flag_c_l(flags, cnt_coincidence, cnt_line, file_name);

    fclose(fp);
    free(line_in_file);
    regfree(&reegex);
  }
}

void s21_grep_logik(options *flags, char *line_in_file, char *file_name,
                    int cnt_line, char **argv, char *pattern, char *str) {
  if (flags->flag_c == 0 && flags->flag_l == 0) {
    if (flags->print_file_flag && flags->flag_n && !flags->flag_o) {
      printf("%s:%d:%s", file_name, cnt_line, line_in_file);

    } else if (flags->print_file_flag && flags->flag_n == 0 &&
               flags->flag_o == 0) {
      printf("%s:%s", file_name, line_in_file);
    } else if (flags->print_file_flag == 0 && flags->flag_n && !flags->flag_o) {
      printf("%d:%s", cnt_line, line_in_file);
      if (line_in_file[strlen(line_in_file) - 1] != '\n') {
        printf("\n");
      }

    } else if (flags->print_file_flag && flags->flag_o) {
      print_flag_o(line_in_file, argv, optind, flags, file_name, cnt_line,
                   pattern, str);
    } else if (flags->print_file_flag == 0 &&
               ((flags->flag_o) || (flags->flag_o && flags->flag_e) ||
                (flags->flag_o && flags->flag_i))) {
      print_flag_o(line_in_file, argv, optind, flags, file_name, cnt_line,
                   pattern, str);
    } else if (flags->print_file_flag && flags->flag_e) {
      printf("%s:%s", file_name, line_in_file);
    } else if (flags->print_file_flag == 0 &&
               ((flags->flag_e) || (flags->flag_e && flags->flag_i))) {
      printf("%s", line_in_file);
      if (line_in_file[strlen(line_in_file) - 1] != '\n') {
        printf("\n");
      }
      if (!flags->flag_n && !flags->flag_c && !flags->flag_l &&
          !flags->flag_e) {
        printf("%s", line_in_file);
        if (line_in_file[strlen(line_in_file) - 1] != '\n') printf("\n");
      }
    } else if (flags->print_file_flag && flags->flag_f) {
      printf("%s:%s", file_name, line_in_file);
    } else if (flags->print_file_flag == 0 && flags->flag_f) {
      if (!flags->flag_n && !flags->flag_c && !flags->flag_l) {
        printf("%s", line_in_file);
        if (line_in_file[strlen(line_in_file) - 1] != '\n') printf("\n");
      }
    } else {
      if (!flags->flag_n && !flags->flag_c && !flags->flag_l) {
        printf("%s", line_in_file);
        if (line_in_file[strlen(line_in_file) - 1] != '\n') printf("\n");
      }
    }
  }
}

void print_flag_o(char *line_in_file, char **argv, int optind, options *flags,
                  char *file_name, int cnt_line, char *pattern, char *str) {
  int len = strlen(argv[optind]);
  if (flags->print_file_flag == 0) {
    if (flags->flag_n) {
      printf("%d:", cnt_line);
      flag_o_logik(line_in_file, argv, optind, len);
    } else if (flags->flag_e) {
      while (*line_in_file) {
        if (strstr(line_in_file, pattern) == line_in_file) {
          printf("%s\n", pattern);
          line_in_file += len;
        } else {
          line_in_file++;
        }
      }
    } else if (flags->flag_i) {
      printf("%s\n", str);
    } else {
      flag_o_logik(line_in_file, argv, optind, len);
    }
  } else if (flags->print_file_flag) {
    printf("%s:", file_name);
    flag_o_logik(line_in_file, argv, optind, len);
  }
}

void flag_o_logik(char *line_in_file, char **argv, int optind, int len) {
  while (*line_in_file) {
    if (strstr(line_in_file, argv[optind]) == line_in_file) {
      printf("%s\n", argv[optind]);
      line_in_file += len;
    } else {
      line_in_file++;
    }
  }
}

void print_flag_c_l(options *flags, int cnt_coincidence, int cnt_line,
                    char *file_name) {
  if (flags->flag_c && flags->flag_l == 0) {
    if (flags->print_file_flag) {
      printf("%s:%d\n", file_name, cnt_coincidence);
    } else {
      printf("%d\n", cnt_coincidence);
    }
  }
  if (flags->flag_c && flags->flag_l) {
    if (cnt_line == 0) {
      cnt_line = 0;
    } else {
      cnt_line = 1;
    }
    if (flags->print_file_flag) {
      printf("%s:%d\n", file_name, cnt_line);
    } else {
      printf("%d\n", cnt_line);
    }
  }
}

int error(options *flags, int argc) {
  if ((flags->flag_error || argc == 2) && flags->flag_s == 0) {
    printf(

        " usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
        "[-C[num]] \n\t[-e pattern] [-f file] [--binary-files=value] "
        "[--color=when]\n\t[--context[=num]] [--directories=action] "
        "[--label] "
        "[--line-buffered]\n\t[--null] [pattern] [file ...]\n");
  }
  return 0;
}
