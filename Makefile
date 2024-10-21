# Makefile for file comparison project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror

# Target executable name
TARGET = fileComparison.out

# Source file
SRC = fileComparison.c

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)

.PHONY: all clean
