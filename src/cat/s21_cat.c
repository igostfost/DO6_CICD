#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int bflag = 0, eflag = 0, nflag = 0, sflag = 0, tflag = 0, vflag = 0,
      school21 = 0;

  const struct option long_options[] = {
      {"number-nonblank", no_argument, &bflag, 1},
      {"number", no_argument, &nflag, 1},
      {"squeeze-blank", no_argument, &sflag, 1},
      {"school21", no_argument, &school21, 1},
      {NULL, 0, NULL, 0}};
  const char *short_options = "+benstvET";

  int checkFlag = 0;
  if (argc == 1) {
    printf("S21_cat: No options. Available options are [benstv]. Try again\n");
  } else {
    while ((checkFlag = getopt_long(argc, argv, short_options, long_options,
                                    NULL)) != -1) {
      switch (checkFlag) {
        case 'b':
          bflag = 1;
          break;
        case 'e':
          eflag = 1;
          vflag = 1;
          break;
        case 'E':
          eflag = 1;
          break;
        case 'n':
          nflag = 1;
          break;
        case 's':
          sflag = 1;
          break;
        case 't':
          tflag = 1;
          vflag = 1;
          break;
        case 'T':
          tflag = 1;
          break;
        case 'v':
          vflag = 1;
          break;
      }
    }

    if (bflag == 1 && nflag == 1) {
      nflag = 0;
    }

    files_output(argc, argv, bflag, eflag, nflag, sflag, tflag, vflag, optind);
  }
  return 0;
}

void printcat(char *file_name, int bflag, int eflag, int nflag, int sflag,
              int tflag, int vflag) {
  FILE *fp;
  if ((fp = fopen(file_name, "r")) == NULL) {
    fprintf(stderr, "cat: %s No such file or directory\n", file_name);
  } else {
    char symbol = fgetc(fp);
    int cnt_line = 0, empty_line_flag, prev_empty_line_flag = 0;
    char prev_symbol = '\n';

    while (symbol != EOF) {
      // proverka pystih strok
      if (symbol == '\n' && prev_symbol == '\n') {
        empty_line_flag = 1;
      } else {
        empty_line_flag = 0;
      }
      // nnymeraci9 strok - flagi n,b
      if (!(sflag && prev_empty_line_flag && empty_line_flag)) {
        if (prev_symbol == '\n') {
          if (nflag == 1 || (bflag == 1 && empty_line_flag == 0)) {
            printf("%6d\t", ++cnt_line);
          }
        }

        if (eflag == 1 && symbol == '\n') {
          printf("$\n");
        } else if (tflag == 1 && symbol == '\t') {
          printf("^I");
        } else if (vflag && symbol < 32 && symbol != '\n' && symbol != '\t') {
          printf("^%c", symbol + 64);
        } else if (vflag && symbol == 127) {
          printf("^?");
        } else {
          printf("%c", symbol);
        }
      }

      prev_symbol = symbol;
      prev_empty_line_flag = empty_line_flag;
      symbol = fgetc(fp);
    }
    fclose(fp);
  }
}

void files_output(int argc, char **argv, int bflag, int eflag, int nflag,
                  int sflag, int tflag, int vflag, int optind) {
  for (int i = optind; i < argc; i++) {
    printcat(argv[i], bflag, eflag, nflag, sflag, tflag, vflag);
  }
}
