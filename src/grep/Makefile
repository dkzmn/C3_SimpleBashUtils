FLAGS=-Wall -Wextra -Werror -std=c11

rebuild: clean all

all: s21_grep

s21_grep:
	gcc $(FLAGS) ./s21_grep.c ../common/common.c -o ./s21_grep

clean:
	rm -rf ./s21_grep