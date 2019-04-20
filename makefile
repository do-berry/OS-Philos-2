CC=g++
DEFINES =
CFLAGS = -std=c++11
LIB = -lpthread -lncurses

main: Filozof.o main.o
	$(CC) -o main main.o Filozof.o $(LIB)

main.o: main.cpp Filozof.h
	$(CC) -c $(DEFINES) $(CFLAGS) main.cpp -o main.o

Filozof.o: Filozof.cpp Filozof.h
	$(CC) -c $(DEFINES) $(CFLAGS) Filozof.cpp -o Filozof.o
