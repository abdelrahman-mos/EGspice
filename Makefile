CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -Iinclude -g #-fsanitize=address 

# Find all .cpp files in src/ and subdirs
SRC = $(shell find src -name '*.cpp')

# Replace src/ with build/ and .cpp with .o
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

TARGET = build/EGspice

.PHONY: all clean run

all: $(TARGET)

# Link all object files into final app
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ -lm

# Rule to compile .cpp into .o, creating subdirs if needed
build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

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