FLAGS=-Wall -Wextra -Werror -std=c11

rebuild: clean all

all: s21_cat

s21_cat:
	gcc $(FLAGS) ./s21_cat.c ../common/common.c -o ./s21_cat

clean:
	rm -rf ./s21_cat