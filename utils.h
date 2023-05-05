#ifndef N_QUEENS_UTILS_H
#define N_QUEENS_UTILS_H

#endif //N_QUEENS_UTILS_H

#include <stdbool.h>

bool isValidPlacement(int placedQueens[], int currCol, int rowToInsert);
bool isValidSolution(int queensPlacement[], int n);

void printSolution(int finalSolution[], int n);
int *intdump(int const *src, size_t len);