#include <iostream>
#include <cstdlib>
#include "stdio.h"

// In-house allocation routine
// Simply checks to ensure that memory is available (probably takes a performance hit, too).

double* AllocDouble(int size) {
    double* thearray = (double*)malloc(sizeof(double) * size);

    if (thearray == NULL) {
        printf("Allocation bombed.\n");
        printf(" size = %3.2f bits (%3.2f MB)\n", (double)size * sizeof(double),
               sizeof(double) * (double)size / (1024.0 * 1024.0));
        exit(1);
    }
    return thearray;
}
