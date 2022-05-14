OBJS=commonfunctions.o temporal_graph.o online_search.o main.o
CC=g++
CFLAGS=-c -std=c++11 -O3

main:$(OBJS)
	$(CC) $^ -o main -O3
%.o:source/%.cpp
	$(CC) $^ $(CFLAGS)

clean:
	$(RM) *.o main