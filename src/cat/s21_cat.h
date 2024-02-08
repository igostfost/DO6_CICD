#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void printcat(char *file_name, int bflag, int eflag, int nflag, int sflag,
              int tflag, int vflag);

void files_output(int argc, char **argv, int bflag, int eflag, int nflag,
                  int sflag, int tflag, int vglag, int optind);

#endif  // SRC_CAT_S21_CAT_H_