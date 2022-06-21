CC = gcc
CFLAGS = -O1 -g -Wall

SRCS := $(shell find ./src/ -name "*.c")

OBJS := $(SRCS:%.c=%.o)

EXE := main

all:$(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

%.o:%.c
	$(CC) -c $^ -o $@ -g

show:
	@echo $(SRCS)

clean:
	rm -f $(OBJS) $(EXE)
