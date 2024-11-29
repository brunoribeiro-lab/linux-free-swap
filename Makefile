CC = gcc
CFLAGS = -Wall -O2 -I./src -Wno-unused-result
TARGET = ./bin/dropcaches

SRC = dropcaches.c \
      src/config/config.c \
      src/cache/cache.c \
      src/process/fileOperationRunning.c \
      src/process/processRunning.c \
      src/swap/swap.c \
      src/updateSystem/updateSystem.c \
      src/utils/ini.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p ./bin
	$(CC) $(CFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
