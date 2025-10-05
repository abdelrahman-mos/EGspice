CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g

# Find all .c files in src/ and subdirs
SRC = $(shell find src -name '*.c')

# Replace src/ with build/ and .c with .o
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

TARGET = build/EGspice

.PHONY: all clean run

all: $(TARGET)

# Link all object files into final app
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -lm

# Rule to compile .c into .o, creating subdirs if needed
build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build
	rm *.log