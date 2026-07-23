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

test: test-busca test-cadastro test-persistencia

test-busca:
	$(CC) $(CFLAGS) -o test_busca tests/test_busca_validacao.c src/busca.c src/validacao.c
	./test_busca

test-cadastro:
	$(CC) $(CFLAGS) -o test_cadastro tests/test_cadastro.c src/cadastro.c src/persistencia.c
	./test_cadastro

test-persistencia:
	$(CC) $(CFLAGS) -o test_persistencia tests/test_persistencia.c src/persistencia.c
	./test_persistencia

clean:
	rm -f src/*.o $(TARGET) test_busca test_cadastro test_persistencia \
		test_busca.exe test_cadastro.exe test_persistencia.exe \
		test_runner test_runner.exe agenda.exe

.PHONY: all clean test test-busca test-cadastro test-persistencia
