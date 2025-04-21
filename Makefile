CC = gcc
SRC = main.c func-tetris.c
LDFLAGS = -lncurses
BIN = tetris

all: $(BIN)
	@echo "Compiling complete"
	./$(BIN)

$(BIN): $(SRC)
	@echo "Compiling program..."
	@$(CC) $(SRC) -o $(BIN) $(LDFLAGS) -O2 -g

clean:
	rm -rf $(BIN)
	@echo "Cleaning complete"

.PHONY: all clean
