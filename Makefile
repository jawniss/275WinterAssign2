CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=
PROGS= dijkstra server
OBJS= server.o dijkstra.o digraph.o

# executable targets
all: dijkstra server

dijkstra: dijkstra.o heap.o digraph.o
	$(CC) dijkstra.o heap.o digraph.o -o dijkstra $(LFLAGS)

server: server.o dijkstra.o digraph.o
	$(CC) server.o dijkstra.o digraph.o -o server $(LFLAGS)

# object targets

server.o: server.cpp digraph.h wdigraph.h heap.h
	$(CC) student_hash.cpp -o student_hash.o $(CFLAGS)

dijkstra.o: dijkstra.cpp digraph.h wdigraph.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
