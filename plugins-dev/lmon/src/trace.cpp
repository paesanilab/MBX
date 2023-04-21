/* Simple routine to return the trace of a SQUARE matrix
 * (need not be symmetric...no symmetry exploited)
 *
 * returns Tr(A)
 *
 */

double trace(double* A, int dim) {
    double trace = 0.0;
    for (int i = 0; i < dim; i++) trace += A[i * dim + i];

    return trace;
}
