#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool isValidPlacement(int placedQueens[], int currCol, int rowToInsert) {
    for (int i = 0; i < currCol; i++) {
        // Checks for horizontal attacks
        if (placedQueens[i] == rowToInsert) return false;

        // Checks for diagonal attacks
        if (abs(placedQueens[i] - rowToInsert) == (currCol - i)) return false;
    }

    return true;
}


/**
 * Checks if a solution is valid, given an array of queens, and the board size
 *
 * @param queensPlacement A pointer to an array containing N integers.
 *      Each position of the array refers to the column, and the value in the position refers to the queen row minus 1
 *      E.g.: an array [0, 3, 1, 2] refers to a board with queens on positions a1, b4, c2, and d3
 * @param n The size of the board
 * @return boolean indicating if 'queensPlacement' is a valid solution
 */
bool isValidSolution(int queensPlacement[], int n) {
    for (int i = 0; i < n; i++) {
        if (!isValidPlacement(queensPlacement, i, queensPlacement[i])) return false;
    }

    return true;
}


void printSolution(int finalSolution[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < finalSolution[i]; j++)
            printf(". ");

        printf("Q ");

        for (int j = finalSolution[i] + 1; j < n; j++)
            printf(". ");

        printf("\n");
    }
}