CC = gcc
EXEC = thread
CFLAGS = -pthread -std=c99

all: ; $(CC) $(CFLAGS) -o $(EXEC) thread.c
