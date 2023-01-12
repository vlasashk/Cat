CC = gcc
CFLAGS = -Wall -Werror -Wextra
SOURCES = s21_cat.c
OBJECTS = $(SOURCES:.c = .o)

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(OBJECTS) -o s21_cat

test:
	clang-format -n $(SOURCES)

linter:
	clang-format -i $(SOURCES)

rebuild: clean all

clean: 
	rm -rf *.o s21_cat
