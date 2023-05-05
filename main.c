#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <omp.h>
#include <mpi/mpi.h>

#include "utils.h"

int numSolutions = 0;

void firstQueen(int rank, int n, int t);
void placeQueen(int placedQueens[], int currCol, int insertRow, int size);

/*
 * Params
 */
int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 8;
    int t = argc > 3 ? atoi(argv[3]) : 8;
    omp_set_num_threads(t);

    int worldSize, worldRank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    printf("Valor de N: %d\n", n);

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    firstQueen(worldRank, n, t);

    int a = 0;
    MPI_Reduce(&numSolutions, &a, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (worldRank == 0) {

        printf("Numero de soluções encontradas: %d\n", numSolutions);
        printf("o outro Numero de soluções encontradas: %d\n", a);
    }

    MPI_Finalize();

    return 0;
}

void firstQueen(int rank, int n, int t) {
    int *placedQueens = (int *) malloc(n * 1);
    placedQueens[0] = rank;

    printf("Começando o posicionamento no rank: %d\n", rank);

    #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            placeQueen(placedQueens, 1, i, n);
        }

    free(placedQueens);
}

void placeQueen(int pastQueens[], int currCol, int insertRow, int size) {
    int * placedQueens = intdump(pastQueens, currCol + 1);
    
    if (!isValidPlacement(placedQueens, currCol, insertRow)) {
        free(placedQueens);
        return;
    }

    placedQueens[currCol] = insertRow;

    if (currCol == size - 1) {
        printf("O rank %d encontrou uma solução.\n", placedQueens[0]);
        printSolution(placedQueens, size);
        printf("\n\n");

        #pragma omp atomic
        numSolutions++;

        free(placedQueens);
        return;
    }

    for (int i = 0; i < size; i++) {
        placeQueen(placedQueens, currCol + 1, i, size);
    }

    free(placedQueens);
}
