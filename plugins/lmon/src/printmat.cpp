/* Prints a square matrix
 *
 * For now, just prints as square with 3 significant digits.
 *
 *
 * TEMP HACK: Only print upper-left 4x4 square for debugging
 */

#include <stdio.h>  //for printf
#include <cstdlib>

void printmat(double* A, int N) {
    // for(int i=0 ; i<N ; i++)
    for (int i = 0; i < 6; i++) {
        // for(int j=0 ; j<N ; j++)
        for (int j = 0; j < 6; j++) {
            printf("%6.4f ", A[i * N + j]);
        }
        printf("\n");
    }
}
