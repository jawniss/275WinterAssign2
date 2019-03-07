CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=
PROGS= count_components
OBJS= digraph.o count_components.o

# executable targets
all: count_components

count_components: count_components.o digraph.o
	$(CC) count_components.o digraph.o -o count_components $(LFLAGS)

# object targets
count_components.o: count_components.cpp digraph.h
	$(CC) count_components.cpp -o count_components.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
