# Simple Makefile for assembler project

# Compiler settings
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -Wextra

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

.PHONY: clean run