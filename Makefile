CC = g++
OBJS = dijkstra.o server.o digraph.o
CFLAGS = -c -Wall -std=c++11 -O3
LFLAGS =
EXC = server

# Main target that runs when only 'make' is entered in the terminal
all: $(EXC)

# This depends on the object existing.
# Linking the object to the executable.
test: test.cpp heap.h
	$(CC) test.cpp -o test
	./test
# This depends on the object existing.
# Linking the object to the executable.
$(EXC): server.o digraph.o dijkstra.o
	$(CC) server.o digraph.o dijkstra.o -o server $(LFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)
# If the object doesn't exist it runs this target.
# This runs when any of the files listed in the arguments have changed.
# This compiles the dijkstra cpp file into an object.
dijkstra.o: dijkstra.cpp dijkstra.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)
# If the object doesn't exist it runs this target.
# This runs when any of the files listed in the arguments have changed.
# This compiles the server cpp file into an object.
server.o: server.cpp digraph.h wdigraph.h
	$(CC) server.cpp -o server.o $(CFLAGS)
# If the user enters 'make clean' into the terminal.
# This deletes the executables and the objects.
# The @ symbol suppresses the output of an error to the terminal if
# the files to delete do not exist.
clean:
	@rm -f $(OBJS) $(EXC) test