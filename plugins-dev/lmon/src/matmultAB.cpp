/* Simple routine to multiply two SQUARE matrices
 * (need not be symmetric...no symmetry exploited)
 *
 * C = A*B
 *
 * (Note: C is over-written, not additive)
 */

void matmultAB(double* A, double* B, double* C, int dim) {
    // Zero C:
    for (int i = 0; i < (dim * dim); i++) C[i] = 0.0;

    // C_ij = sum_k (A_ik B_kj)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                C[i * dim + j] += A[i * dim + k] * B[k * dim + j];
            }
        }
    }
}
