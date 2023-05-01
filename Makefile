all: n_queens


n_queens: main.o utils.o
	mpicc -o n_queens main.o utils.o -lpthread -fopenmp

utils.o: utils.c
	gcc -o utils.o -c utils.c -Wall

main.o: main.c utils.h
	gcc -o main.o -c main.c -Wall -fopenmp


clean:
	rm -rf *.o
