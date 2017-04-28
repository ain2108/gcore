CC=g++
CFLAGS= -fconcepts -std=c++1z
SRCS = $(wildcard *.c)

BIN=bin

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS)  -o $(BIN)/$@ $<

clean:
	rm $(BIN)/*