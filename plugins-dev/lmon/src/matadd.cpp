/* Simple routine to copy two matrices
 * (need not be symmetric...no symmetry exploited)
 *
 * C = A+B
 *
 * (Note: C is overwritten, not incremented)
 */

void matadd(double* A, double* B, double* C, int dim) {
    for (int i = 0; i < (dim * dim); i++) C[i] = A[i] + B[i];
}
