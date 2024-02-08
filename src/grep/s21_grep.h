#ifndef SRC_GREP_S21_CAT_H_
#define SIZE 4096

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
  int flag_error;
  int print_file_flag;
} options;

void finder_flags(int argc, char **argv, char *pattern, options *flags);

void print_file_flag(options *flags, int optind, int argc);

void files_output_2(int argc, char **argv, options *flags, int optind,
                    char *pattern);

void print_grep_2(char *file_name, char *pattern, char **argv, options *flags);

void s21_grep_logik(options *flags, char *line_in_file, char *file_name,
                    int cnt_line, char **argv, char *pattern, char *str);

void print_flag_o(char *line_in_file, char **argv, int optind, options *flags,
                  char *file_name, int cnt_line, char *pattern, char *str);

void flag_o_logik(char *line_in_file, char **argv, int optind, int len);

void print_flag_c_l(options *flags, int cnt_coincidence, int cnt_line,
                    char *file_name);

int error(options *flags, int argc);

#endif  // SRC_GREP_S21_CAT_H_
