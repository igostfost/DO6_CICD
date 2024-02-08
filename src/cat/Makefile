CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: s21_cat

rebuild: clean all

s21_cat: s21_cat.h s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -f s21_cat
