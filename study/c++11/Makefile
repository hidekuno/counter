TARGET = tree
CFLAGS = --std=c++11 -c -O2
LDFLAGS = 
CC = g++

all: $(TARGET)

tree:	tree.o
	$(CC) $(LDFLAGS) tree.o -o tree

tree.o:	tree.cpp
	$(CC) $(CFLAGS) tree.cpp

clean:
	rm -f $(TARGET) *.o *.class

class:
	$(CC) $(CFLAGS) -fdump-class-hierarchy tree.cpp
