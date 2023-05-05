#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <omp.h>
#include <mpi/mpi.h>

#include "utils.h"

int numSolutions = 0;
int printSolutions = 0;

void firstQueen(int rank, int n, int t, int offset);
void placeQueen(int placedQueens[], int currCol, int insertRow, int size);

/*
 * Params
 */
int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 8;
    int t = argc > 2 ? atoi(argv[2]) : 8;
    printSolutions = argc > 3 ? atoi(argv[3]) : 0;


    omp_set_num_threads(t);

    int worldSize, worldRank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    double start = MPI_Wtime();

    int finalNumSolutions = 0;
    if (worldSize < n) {
        for (int i = 0; i < worldSize % n; i++) {
            MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

            MPI_Barrier(MPI_COMM_WORLD);

            firstQueen(worldRank, n, t, worldSize * i);

            MPI_Reduce(&numSolutions, &finalNumSolutions, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);

        firstQueen(worldRank, n, t, 0);

        MPI_Reduce(&numSolutions, &finalNumSolutions, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    double end = MPI_Wtime();

    if (worldRank == 0) {
        printf("Soluções para %d damas\n", n);
        printf("Usando %d threads\t e %d proceessos\n", t, worldSize);
        printf("Número de soluções encontradas: %d\n", finalNumSolutions);
        printf("Tempo: %f\n", end - start);
    }

    MPI_Finalize();

    return 0;
}

void firstQueen(int rank, int n, int t, int offset) {
    int *placedQueens = (int *) malloc(n * 1);

    // If we have more processes than Queens (or it's not the first pass), just
    //  search the solution in the ranks that will stay lower than n
    if (rank + offset >= n) return;

    placedQueens[0] = rank + offset;

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
        if (printSolutions) {
            printf("O rank %d encontrou uma solução.\n", placedQueens[0]);
            printSolution(placedQueens, size);
            printf("\n\n");
        }

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
