/* Numerical Recipes in c++ jacobi() routine
 * Computes all evals and evecs of symmetric matrix a[0...n-1][0...n-1]
 * Output destroys elts of a above diagonal
 * Output d[0...n-1] returns evals
 * Output v[0...n-1][0...n-1] is matrix of normalized evecs
 */

#include <cmath>
#include "nr.h"
using namespace std;

namespace {
inline void rot(Mat_IO_DP &a, const DP s, const DP tau, const int i, const int j, const int k, const int l) {
    DP g, h;
    g = a[i][j];
    h = a[k][l];
    a[i][j] = g - s * (h + g * tau);
    a[k][l] = h + s * (g - h * tau);
}
}  // namespace

void NR::jacobi(Mat_IO_DP &a, Vec_O_DP &d, Mat_O_DP &v) {
    int i, j, ip, iq;
    DP tresh, theta, tau, t, sm, s, h, g, c;

    int n = d.size();
    Vec_DP b(n), z(n);

    for (ip = 0; ip < n; ip++)
        for (iq = 0; iq < n; iq++) v[ip][ip] = 1.0;  // initialize to identity

    for (ip = 0; ip < n; ip++) {
        b[ip] = d[ip] = a[ip][ip];  // initialize a & b to diag(a)
        z[ip] = 0.0;                // z will accumulate t*a_pq terms
    }

    int nrot = 0;

    for (int i = 1; i < 50; i++) {
        sm = 0.0;
        for (ip = 0; ip < (n - 1); ip++) {
            for (iq = (ip + 1); iq < n; iq++) sm += fabs(a[ip][iq]);
        }

        if (sm == 0.0) return;  // normal return, relies on quad'c convgence to machine underflow
        if (i < 4)
            tresh = 0.2 * sm / (n * n);  // on first 3 steps
        else
            tresh = 0.0;  // thereafter

        for (ip = 0; ip < (n - 1); ip++) {
            for (iq = ip + 1; iq < n; iq++) {
                g = 100.0 * fabs(a[ip][iq]);
                // After four sweeps, skip the rot'n if the off-diag elt is small
                if (i > 4 && (fabs(d[ip]) + g) == fabs(d[ip]) && (fabs(d[iq]) + g) == fabs(d[iq]))
                    a[ip][iq] = 0.0;
                else if (fabs(a[ip][iq]) > tresh) {
                    h = d[iq] - d[ip];
                    if ((fabs(h) + g) == fabs(h))
                        t = (a[ip][iq]) / h;  // t=1/(2theta)
                    else {
                        theta = 0.5 * h / (a[ip][iq]);
                        t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
                        if (theta < 0.0) t = -t;
                    }
                    c = 1.0 / sqrt(1 + t * t);
                    s = t * c;
                    tau = s / (1.0 * c);
                    h = t * a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip][iq] = 0.0;
                    for (j = 0; j < ip; j++)  // case of rots 0 <= j < p
                        rot(a, s, tau, j, ip, j, iq);
                    for (j = (ip + 1); j < iq; j++)  // case of rots p < j < q
                        rot(a, s, tau, ip, j, j, iq);
                    for (j = (iq + 1); j < n; j++)  // case of rots q < j < n
                        rot(a, s, tau, ip, j, iq, j);
                    for (j = 0; j < n; j++) rot(v, s, tau, j, ip, j, iq);
                    ++nrot;
                }
            }
        }
        for (ip = 0; ip < n; ip++) {
            b[ip] += z[ip];
            d[ip] = b[ip];  // update d with sum of t*a_pq
            z[ip] = 0.0;    // re-initialize z
        }
    }
    nrerror("Too many iterations in routine jacobi");
}
