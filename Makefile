CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = agenda

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(OBJ)
	$(CC) $(CFLAGS) -o test_runner tests/test_busca_validacao.c src/busca.c src/validacao.c
	./test_runner

clean:
	rm -f src/*.o $(TARGET) test_runner test_runner.exe agenda.exe

.PHONY: all clean test
