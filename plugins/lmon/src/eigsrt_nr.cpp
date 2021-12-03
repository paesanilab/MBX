/* Numerical Recipes routine to sort eigenvalues
 * Given evals d[0,...,n-1] and evecs v[0,...,n-1][0,...,n-1]
 * as output from jacob(), this routine sorts the eigenvalues
 * into descending order and rearranges the columns of v correspondingly.
 * The method is straight insertion and order (N^2)
 */

#include "nr.h"
void NR::eigsrt(Vec_IO_DP &d, Mat_IO_DP &v) {
    int i, j, k;
    DP p;

    int n = d.size();
    for (i = 0; i < (n - 1); i++) {
        p = d[k = i];
        for (j = i; j < n; j++)
            if (d[j] >= p) p = d[k = j];
        if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j++) {
                p = v[j][i];
                v[j][i] = v[j][k];
                v[j][k] = p;
            }
        }
    }
}
