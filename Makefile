CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g -fsanitize=address 

# Find all .c files in src/ and subdirs
SRC = $(shell find src -name '*.cpp')

# Replace src/ with build/ and .c with .o
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

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
	rm *.op*
	rm *.ac*

install: $(TARGET)
	@echo "Installing EGspice to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed as 'ُEGspice'. You can now run it using 'EGspice' in the terminal."

uninstall:
	@echo "Removing installed EGspice..."
	sudo rm -f /usr/local/bin/EGspice
	@echo "EGspice removed."