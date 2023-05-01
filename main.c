#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <omp.h>

#include "utils.h"

bool solutionFound;

bool placeQueen(int placedQueens[], int currCol, int insertRow, int size);

/*
 * Params
 */
int main(int argc, char *argv[]) {
    int n, numThreads, numNodes;
    double start, end;
    solutionFound = false;

    getArgs(argc,  argv, &n, &numThreads, &numNodes);

    int finalSolution[n];

    omp_set_num_threads(numThreads);

    start = omp_get_wtime();

    #pragma omp parallel for shared(solutionFound, n, finalSolution) default(none)
    for (int i = 0; i < n; i++) {
        // In parallel, try all solutions for the first column
        int *placedQueens = (int *) malloc(n * sizeof(int));

        placeQueen(placedQueens, 0, i, n);

        if (isValidSolution(placedQueens, n)) {
            memcpy(finalSolution, placedQueens, n * sizeof(int));
        }
        free(placedQueens);
    }

    end = omp_get_wtime();
    printf("Time taken with %d threads: %4.2f\n", numThreads, end - start);

    printSolution(finalSolution, n);
//    for (int j = 0; j < n; j++)
//        printf("Solution[%d] = %d\n", j, finalSolution[j]);

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
