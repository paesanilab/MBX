// Simple routine to diagonalize a matrix, returning
// both evals and evecs (sorted descending).
// Uses jacobi() and eigsrt() routines from Numerical Recipes.

#include "nr.h"
#include "vib.h"
void diagon(Mat_IO_DP &a, Vec_IO_DP &d, Mat_IO_DP &v) {
    jacobi(a, d, v);
    eigsrt(d, v);
}
