CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
SRC = src/ls.c
OBJ = $(SRC:.c=.o)
BIN = bin/ls

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f $(OBJ) $(BIN)
