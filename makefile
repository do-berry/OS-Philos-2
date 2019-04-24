CC=g++
DEFINES =
CFLAGS = -std=c++11
LIB = -lpthread -lncurses

main: Philo.o main.o
	$(CC) -o main main.o Philo.o $(LIB)

main.o: main.cpp Philo.h
	$(CC) -c $(DEFINES) $(CFLAGS) main.cpp -o main.o

Philo.o: Philo.cpp Philo.h
	$(CC) -c $(DEFINES) $(CFLAGS) Philo.cpp -o Philo.o
