CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDLIBS = -lm
# LDFLAGS = -L./library -I./library

CODEDIR = src

SRCS=$(wildcard $(CODEDIR)/*.c)

.PHONY: all clean

all: main.c
	$(CC) $(CFLAGS) $(SRCS) -o Catan $(LDLIBS)

clean:
	rm Catan
