CC=g++
CFLAGS= -fconcepts -std=c++1z
SRCS = $(wildcard *.c)

BIN=bin


PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS)  -o $(BIN)/$@ $<

clean:
	rm $(BIN)/*