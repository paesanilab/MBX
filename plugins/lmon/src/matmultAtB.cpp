/* Simple routine to multiply two SQUARE matrices
 * (need not be symmetric...no symmetry exploited)
 *
 * C = A^t*B
 *
 * (Note: C is over-written, not additive)
 */

void matmultAtB(double* A, double* B, double* C, int dim) {
    // Zero C:
    for (int i = 0; i < (dim * dim); i++) C[i] = 0.0;

    // C_ij = sum_k ((A^t)_ik B_kj)
    //     = sum_k (    A_ki B_kj)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                C[i * dim + j] += A[k * dim + i] * B[k * dim + j];
            }
        }
    }
}
