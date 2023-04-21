/* Simple routine to copy two matrices
 * (need not be symmetric...no symmetry exploited)
 *
 * A --(copied)--> B
 *
 */

void matcopy(double* A, double* B, int dim) {
    for (int i = 0; i < (dim * dim); i++) B[i] = A[i];
}
