# Makefile for SwapClearance

CC = gcc
CFLAGS = -Wall -O2 -I./src -Wno-unused-result
TARGET = dropcaches

SRC = dropcaches.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJ)
