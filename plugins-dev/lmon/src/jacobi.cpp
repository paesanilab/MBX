/* Numerical Recipes in c++ jacobi() routine
 * Computes all evals and evecs of symmetric matrix a[0...n-1][0...n-1]
 * Output destroys elts of a above diagonal
 * Output d[0...n-1] returns evals
 * Output v[0...n-1][0...n-1] is matrix of normalized evecs
 *
 * RPS: Modified to use standard double types instead of crazy NR-specific types
 */

#include <cmath>
#include "nr.h"
#include <cstdlib>
using namespace std;

namespace {
inline void rot(double* a, const double s, const double tau, const int i, const int j, const int k, const int l,
                const int n) {
    double g, h;
    g = a[i * n + j];
    h = a[k * n + l];
    a[i * n + j] = g - s * (h + g * tau);
    a[k * n + l] = h + s * (g - h * tau);
}
}  // namespace

void jacobi(double* a, double* d, double* v, int n) {
    int i, j, ip, iq;
    double tresh, theta, tau, t, sm, s, h, g, c;

    double* b = (double*)malloc(sizeof(double) * n);
    double* z = (double*)malloc(sizeof(double) * n);

    for (ip = 0; ip < n; ip++) {
        for (iq = 0; iq < n; iq++) v[ip * n + iq] = 0.0;

        v[ip * n + ip] = 1.0;  // initialize to identity
    }

    for (ip = 0; ip < n; ip++) {
        b[ip] = a[ip * n + ip];  // initialize a & b to diag(a)
        d[ip] = b[ip];           // initialize a & b to diag(a)
        z[ip] = 0.0;             // z will accumulate t*a_pq terms
    }

    int nrot = 0;

    for (int i = 1; i < 50; i++) {
        sm = 0.0;
        for (ip = 0; ip < (n - 1); ip++) {
            for (iq = (ip + 1); iq < n; iq++) sm += fabs(a[ip * n + iq]);
        }

        if (sm == 0.0) return;  // normal return, relies on quad'c convgence to machine underflow

        if (i < 4)
            tresh = 0.2 * sm / (n * n);  // on first 3 steps
        else
            tresh = 0.0;  // thereafter

        for (ip = 0; ip < (n - 1); ip++) {
            for (iq = (ip + 1); iq < n; iq++) {
                g = 100.0 * fabs(a[ip * n + iq]);
                // After four sweeps, skip the rot'n if the off-diag elt is small
                if ((i > 4) && ((fabs(d[ip]) + g) == fabs(d[ip])) && ((fabs(d[iq]) + g) == fabs(d[iq])))
                    a[ip * n + iq] = 0.0;
                else if (fabs(a[ip * n + iq]) > tresh) {
                    h = d[iq] - d[ip];
                    if ((fabs(h) + g) == fabs(h))
                        t = (a[ip * n + iq]) / h;  // t=1/(2theta)
                    else {
                        theta = 0.5 * h / (a[ip * n + iq]);
                        t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
                        if (theta < 0.0) t = -t;
                    }
                    c = 1.0 / sqrt(1 + t * t);
                    s = t * c;
                    tau = s / (1.0 + c);
                    h = t * a[ip * n + iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip * n + iq] = 0.0;
                    for (j = 0; j < ip; j++)  // case of rots 0 <= j < p
                        rot(a, s, tau, j, ip, j, iq, n);
                    for (j = (ip + 1); j < iq; j++)  // case of rots p < j < q
                        rot(a, s, tau, ip, j, j, iq, n);
                    for (j = (iq + 1); j < n; j++)  // case of rots q < j < n
                        rot(a, s, tau, ip, j, iq, j, n);
                    for (j = 0; j < n; j++) rot(v, s, tau, j, ip, j, iq, n);
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
    printf("Too many iterations in routine jacobi");
    exit(0);
}
