# Makefile for the To-Do List C program

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = todo
SOURCE = todo.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean build artifacts
clean:
	rm -f $(TARGET) tasks.dat

# Run tests
test: $(TARGET)
	./$(TARGET) list
	./$(TARGET) add "Test task from Makefile"
	./$(TARGET) list
	./$(TARGET) update 1 1
	./$(TARGET) list
	./$(TARGET) delete 1
	./$(TARGET) list

# Install (copy to system path - optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean test install
