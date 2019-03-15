CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=
PROGS= dijkstra server
OBJS= server.o dijkstra.o digraph.o serialport.o

# executable targets
# main target server
all: server dijkstra

# target dijkstra that links dijkstra.o and digraph.o together
dijkstra: dijkstra.o digraph.o
	$(CC) dijkstra.o digraph.o -o dijkstra $(LFLAGS)

# target server that links server.o, dijkstra.o and digraph.o
server: server.o dijkstra.o digraph.o serialport.o
	$(CC) server.o dijkstra.o digraph.o serialport.o -o server $(LFLAGS)

# object targets

# object target server.o that compiles the object
server.o: server.cpp digraph.h wdigraph.h
	$(CC) server.cpp -o server.o $(CFLAGS)

dijkstra.o: dijkstra.cpp dijkstra.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

serialport.o: serialport.h serialport.cpp
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

# clean which removes objects and executables
clean:
	@rm $(OBJS) $(PROGS)
