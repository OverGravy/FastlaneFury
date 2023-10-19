CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L  -g
LDFLAGS = -pthread -lallegro -lallegro_primitives 

# Define the directories for source files and object files
SRC_DIR := "./"
OBJ_DIR := obj

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

LIBS = $(shell pkg-config --cflags --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_audio-5 allegro_ttf-5 )  # Remove -lX11 from LIBS

TARGET = Highway # Name of the executable		

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS) $(LIBS) -lm

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean