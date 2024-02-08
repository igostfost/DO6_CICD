CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: s21_grep

rebuild: clean all

s21_grep: s21_grep.c
	$(CC) s21_grep.c -o s21_grep

clean:
	rm -f s21_grep