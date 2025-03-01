# Simple Makefile for assembler project

# Compiler settings
CC = gcc
CFLAGS = -Wall -g

# Source and header directories
SRC_DIR = code_files
HEADER_DIR = headers

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Main target - compile the assembler
assembler: $(SRCS)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) $(SRCS) -o assembler

# Clean up
clean:
	rm -f assembler

# Run the assembler with a test file
run: assembler
	./assembler test_file.as

.PHONY: clean run