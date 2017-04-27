CC=g++
FLAGS= -fconcepts -std=c++1z -g
CURR_DIR= $(shell pwd)
GRAPH_LIB_DIR=/home/ubuntu/safe_dir/Graph-Library/new_src
THIRD_PARTY_CODE= $(CURR_DIR)/../new_src/cmcstl2/include
INCLUDE=
EXE=main
DEBUG_EXE=debug
VALGRIND_TARGET=valgrind
OBJS=main.o 

$(EXE): $(OBJS)
	$(CC) $(FLAGS) $(INCLUDE) $(OBJS) -o $@

$(DEBUG_EXE): $(OBJS)
	g++ -fconcepts $(INCLUDE) -O0 -std=c++1z -g3 $(OBJS) -o debug

$(VALGRIND_TARGET): $(DEBUG_EXE)
	valgrind -v --track-origins=yes --num-callers=20 --leak-check=yes --leak-resolution=high --show-reachable=yes ./debug

main.o: main.c
	$(CC) -c $(FLAGS) $(INCLUDE) main.c -o $@

all: clean $(EXE)

.PHONY:clean
clean:
	-@rm -rf *.o $(EXE) $(DEBUG_EXE) 2>/dev/null || true