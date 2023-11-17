CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -g
LDFLAGS = -pthread -lalleg

SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET = Highway
BIN_TARGET = $(BIN_DIR)/$(TARGET)

all: $(BIN_TARGET)

$(BIN_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN_TARGET) $(LDFLAGS) -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(BIN_TARGET)

.PHONY: all clean
