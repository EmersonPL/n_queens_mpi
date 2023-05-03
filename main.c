#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <omp.h>
#include <mpi/mpi.h>

#include "utils.h"

bool solutionFound;

bool placeQueen(int placedQueens[], int currCol, int insertRow, int size);

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

    int *placedQueens = (int *) malloc(n * sizeof(int));
    printf("My rank: %d;\tComm size: %d\n", worldRank, worldSize);
//    #pragma omp parallel for shared(solutionFound, n, finalSolution) default(none)
    if (worldRank == 0) {
        for (int i = 0; i < n; i++) {
            // each node should run the search for a given starting position
            placedQueens[0] = i;

            int dest = i % (worldSize - 1) + 1;
            printf("dest: %d\n", dest);
            MPI_Send(placedQueens, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(placedQueens, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received value %d from process 0\n", worldRank, placedQueens[0]);
    }

//    printSolution(finalSolution, n);

    MPI_Finalize();

    return 0;
}

bool placeQueen(int placedQueens[], int currCol, int insertRow, int size) {
    if (solutionFound) return true;
    if (!isValidPlacement(placedQueens, currCol, insertRow)) return false;

    placedQueens[currCol] = insertRow;

    if (currCol == size - 1) {
        solutionFound = true;
        return true;
    }

    for (int i = 0; i < size; i++) {
        // try all positions for the next column, until one of them leads to a solution
        if (placeQueen(placedQueens, currCol + 1, i, size)) return true;
    }

    return false;
}
