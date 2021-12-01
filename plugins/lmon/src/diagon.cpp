// Simple routine to diagonalize a matrix, returning
// both evals and evecs (sorted descending).
// Uses jacobi() and eigsrt() routines, adapted from Numerical Recipes.

#include "vib.h"
void diagon(double* a, double* d, double* v, int n) {
    jacobi(a, d, v, n);
    eigsrt(d, v, n);
}
