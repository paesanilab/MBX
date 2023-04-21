/* Numerical Recipes routine to sort eigenvalues
 * Given evals d[0,...,n-1] and evecs v[0,...,n-1][0,...,n-1]
 * as output from jacobi(), this routine sorts the eigenvalues
 * into descending order and rearranges the columns of v correspondingly.
 * The method is straight insertion and order (N^2)
 *
 * RPS: Modified to use standard double types instead of crazy NR-specific types
 */

void eigsrt(double* d, double* v, int n) {
    int i, j, k;
    double p;

    // int n=d.size();
    for (i = 0; i < (n - 1); i++) {
        p = d[k = i];
        for (j = i; j < n; j++)
            if (d[j] >= p) p = d[k = j];
        if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j++) {
                // p=v[j][i];
                // v[j][i]=v[j][k];
                // v[j][k]=p;
                p = v[j * n + i];
                v[j * n + i] = v[j * n + k];
                v[j * n + k] = p;
            }
        }
    }
}
