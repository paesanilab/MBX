//
// Copyright (C) 2004, 2005 Chaok Seok, Evangelos Coutsias and Ken Dill
//      UCSF, Univeristy of New Mexico, Seoul National University
// Witten by Chaok Seok and Evangelos Coutsias 2004.
// [this C++ version is by Volodymyr Babin <vb27606@gmail.com> 2011]
// The original version is at http://www.dillgroup.ucsf.edu/rmsd
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA  02110-1301  USA
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include <cmath>
#include <cassert>
#include <algorithm>
#include <stdexcept>

#include "rmsd.h"

#ifndef DISABLE_RESTRICT
#define RESTRICT __restrict__
#else
#define RESTRICT
#endif  // DISABLE_RESTRICT

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

//
// square root of (a*a + b*b) without overflow or destructive underflow
//

double pythag(double a, double b) {
    double t = std::fabs(a);
    double u = std::fabs(b);

    double p = std::max(t, u);
    double q = std::min(t, u);

    if (q == 0.0) return p;

    while (true) {
        double r = q / p;
        r *= r;

        const double t = 4.0 + r;
        if (t == 4.0) break;

        const double s = r / t;
        p += 2 * p * s;
        q *= s;
    }

    return p;
}

//----------------------------------------------------------------------------//

//
// performs Givens rotations to reduce symmetric 4x4 matrix to tridiagonal
// (comments are copied from the FORTRAN code with indices starting from 1)
//

void givens4(const double S[4][4], double T[4][4], double V[4][4]) {
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) {
            T[i][j] = S[i][j];
            V[i][j] = 0.0;
        }

    // zero out entries T(4,1) and T(1,4)
    // compute cos and sin of rotation angle in the 3-4 plane

    double c1(1), s1(0);
    const double r1 = pythag(T[2][0], T[3][0]);

    if (r1 != 0.0) {
        c1 = T[2][0] / r1;
        s1 = T[3][0] / r1;

        V[2][2] = c1;
        V[2][3] = s1;
        V[3][2] = -s1;
        V[3][3] = c1;

        T[2][0] = r1;
        T[3][0] = 0.0;

        double F[2][3];

        // T(3:4,2:4) = matmul(V(3:4,3:4), T(3:4,2:4))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j) {
                F[i][j] = 0.0;
                for (size_t k = 0; k < 2; ++k) F[i][j] += V[i + 2][k + 2] * T[k + 2][j + 1];
            }

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j) T[i + 2][j + 1] = F[i][j];

        // T(1:2,3:4) = transpose(T(3:4,1:2))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 2; j < 4; ++j) T[i][j] = T[j][i];

        // T(3:4,3:4) = matmul(T(3:4,3:4), transpose(V(3:4,3:4)))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) F[i][j] = T[i + 2][j + 2];

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) {
                T[i + 2][j + 2] = 0.0;
                for (size_t k = 0; k < 2; ++k) T[i + 2][j + 2] += F[i][k] * V[j + 2][k + 2];
            }
    }

    // zero out entries T(3,1) and T(1,3)
    // compute cos and sin of rotation angle in the 2-3 plane

    double c2(1), s2(0);
    const double r2 = pythag(T[2][0], T[1][0]);

    if (r2 != 0.0) {
        c2 = T[1][0] / r2;
        s2 = T[2][0] / r2;

        V[1][1] = c2;
        V[1][2] = s2;
        V[2][1] = -s2;
        V[2][2] = c2;

        T[1][0] = r2;
        T[2][0] = 0.0;

        double F[2][3];

        // T(2:3,2:4) = matmul(V(2:3,2:3), T(2:3,2:4))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j) {
                F[i][j] = 0.0;
                for (size_t k = 0; k < 2; ++k) F[i][j] += V[i + 1][k + 1] * T[k + 1][j + 1];
            }

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j) T[i + 1][j + 1] = F[i][j];

        // T(1,2:3)   = T(2:3,1)
        // T(4,2:3)   = T(2:3,4)
        for (size_t j = 1; j < 4; ++j) {
            T[0][j] = T[j][0];
            T[3][j] = T[j][3];
        }

        // T(2:3,2:3) = matmul(T(2:3,2:3), transpose(V(2:3,2:3)))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) F[i][j] = T[i + 1][j + 1];

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) {
                T[i + 1][j + 1] = 0.0;
                for (size_t k = 0; k < 2; ++k) T[i + 1][j + 1] += F[i][k] * V[j + 1][k + 1];
            }
    }

    // zero out entries T(4,2) and T(2,4)
    // compute cos and sin of rotation angle in the 3-4 plane

    double c3(1), s3(0);
    const double r3 = pythag(T[3][1], T[2][1]);

    if (r3 != 0.0) {
        c3 = T[2][1] / r3;
        s3 = T[3][1] / r3;

        V[2][2] = c3;
        V[2][3] = s3;
        V[3][2] = -s3;
        V[3][3] = c3;

        T[2][1] = r3;
        T[3][1] = 0.0;

        double F[2][2];

        // T(3:4,3:4) = matmul(V(3:4,3:4), T(3:4,3:4))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) {
                F[i][j] = 0.0;
                for (size_t k = 0; k < 2; ++k) F[i][j] += V[i + 2][k + 2] * T[k + 2][j + 2];
            }

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) T[i + 2][j + 2] = F[i][j];

        // T(1:2,3:4) = transpose(T(3:4,1:2))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 2; j < 4; ++j) T[i][j] = T[j][i];

        // T(3:4,3:4) = matmul(T(3:4,3:4), transpose(V(3:4,3:4)))
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) F[i][j] = T[i + 2][j + 2];

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j) {
                T[i + 2][j + 2] = 0.0;
                for (size_t k = 0; k < 2; ++k) T[i + 2][j + 2] += F[i][k] * V[j + 2][k + 2];
            }
    }

    // compute net rotation matrix (accumulate similarity for evec. computation)
    // To save transposing later, This is the transpose!

    V[0][0] = 1.0;

    for (size_t j = 1; j < 4; ++j) V[0][j] = V[j][0] = 0.0;

    V[1][1] = c2;
    V[2][1] = c1 * s2;
    V[3][1] = s1 * s2;

    const double c1c2 = c1 * c2;
    const double s1c2 = s1 * c2;

    V[1][2] = -s2 * c3;
    V[2][2] = c1c2 * c3 - s1 * s3;
    V[3][2] = s1c2 * c3 + c1 * s3;
    V[1][3] = s2 * s3;
    V[2][3] = -c1c2 * s3 - s1 * c3;
    V[3][3] = -s1c2 * s3 + c1 * c3;
}

//----------------------------------------------------------------------------//

void svdcmp(double a[4][4], double w[4], double v[4][4]) {
    using std::fabs;
    using std::max;
    using std::sqrt;
    //    using std::copysign;

    const int N = 4;

    int l;

    double f(0), h(0);
    double g(0), s(0), scale(0);
    double anorm(0), rv1[2 * N];

    for (int i = 0; i < N; ++i) {
        l = i + 1;
        rv1[i] = scale * g;

        g = s = scale = 0.0;

        for (int k = i; k < N; ++k) scale += fabs(a[k][i]);

        if (scale != 0.0) {
            for (int k = i; k < N; ++k) {
                a[k][i] /= scale;
                s += a[k][i] * a[k][i];
            }

            f = a[i][i];
            g = -copysign(sqrt(s), f);
            h = f * g - s;
            a[i][i] = f - g;

            for (int j = l; j < N; ++j) {
                s = 0.0;
                for (int k = i; k < N; ++k) s += a[k][i] * a[k][j];
                f = s / h;
                for (int k = i; k < N; ++k) a[k][j] += f * a[k][i];
            }

            for (int k = i; k < N; ++k) a[k][i] *= scale;

        }  // scale != 0.0

        w[i] = scale * g;
        g = s = scale = 0.0;

        if (l < N) {  // ?
            for (int k = l; k < N; ++k) scale += fabs(a[i][k]);

            if (scale != 0.0) {
                for (int k = l; k < N; ++k) {
                    a[i][k] /= scale;
                    s += a[i][k] * a[i][k];
                }

                f = a[i][l];
                g = -copysign(sqrt(s), f);
                h = f * g - s;

                a[i][l] = f - g;

                for (int k = l; k < N; ++k) rv1[k] = a[i][k] / h;

                for (int j = l; j < N; ++j) {
                    s = 0.0;
                    for (int k = l; k < N; ++k) s += a[j][k] * a[i][k];
                    for (int k = l; k < N; ++k) a[j][k] += s * rv1[k];
                }

                for (int k = l; k < N; ++k) a[i][k] *= scale;
            }  // scale != 0.0
        }      // l < N

        anorm = max(anorm, (fabs(w[i]) + fabs(rv1[i])));
    }

    for (int i = N - 1; i >= 0; --i) {
        if (i != N - 1) {
            if (g != 0.0) {
                for (int j = l; j < N; ++j) v[j][i] = (a[i][j] / a[i][l]) / g;

                for (int j = l; j < N; ++j) {
                    s = 0.0;
                    for (int k = l; k < N; ++k) s += a[i][k] * v[k][j];
                    for (int k = l; k < N; ++k) v[k][j] += s * v[k][i];
                }
            }  // g != 0.0

            for (int j = l; j < N; ++j) v[i][j] = v[j][i] = 0.0;
        }  // i != N - 1

        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }

    for (int i = N - 1; i >= 0; --i) {
        l = i + 1;
        g = w[i];

        for (int j = l; j < N; ++j) a[i][j] = 0.0;

        if (g != 0.0) {
            g = 1.0 / g;

            for (int j = l; j < N; ++j) {
                s = 0.0;
                for (int k = l; k < N; ++k) s += a[k][i] * a[k][j];
                f = (s / a[i][i]) * g;
                for (int k = i; k < N; ++k) a[k][j] += f * a[k][i];
            }

            for (int j = i; j < N; ++j) a[j][i] *= g;
        } else {
            for (int j = i; j < N; ++j) a[j][i] = 0.0;
        }  // g != 0.0

        a[i][i] += 1.0;
    }

    int nm(0);
    double c(0), x(0), y(0), z(0);

    for (int k = N - 1; k >= 0; --k) {
        for (int its = 0; its < 30; ++its) {
            for (l = k; l >= 0; --l) {
                nm = l - 1;
                if ((fabs(rv1[l]) + anorm) == anorm) goto l2;

                assert(nm >= 0);
                if ((fabs(w[nm]) + anorm) == anorm) goto l1;
            }

        l1:
            c = 0.0;
            s = 1.0;

            assert(l >= 0);
            for (int i = l; i <= k; ++i) {
                f = s * rv1[i];
                rv1[i] *= c;
                if ((fabs(f) + anorm) == anorm) goto l2;
                g = w[i];
                h = pythag(f, g);
                w[i] = h;
                h = 1.0 / h;
                c = (g * h);
                s = -(f * h);
                assert(nm >= 0);
                for (int j = 0; j < N; ++j) {
                    y = a[j][nm];
                    z = a[j][i];
                    a[j][nm] = (y * c) + (z * s);
                    a[j][i] = -(y * s) + (z * c);
                }
            }

        l2:
            z = w[k];
            if (l == k) {
                if (z < 0.0) {
                    w[k] = -z;
                    for (int j = 0; j < N; ++j) v[j][k] = -v[j][k];
                }
                goto l3;
            }
            if (its == 30) throw std::runtime_error("rmsd_tools::rmsd: no convergence in svdcmp()");
            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
            g = pythag(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + copysign(g, f))) - h)) / x;
            c = s = 1.0;

            for (int j = l; j <= nm; ++j) {
                const int i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = pythag(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = (x * c) + (g * s);
                g = -(x * s) + (g * c);
                h = y * s;
                y = y * c;
                for (int jj = 0; jj < N; ++jj) {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = (x * c) + (z * s);
                    v[jj][i] = -(x * s) + (z * c);
                }
                z = pythag(f, h);
                w[j] = z;
                if (z != 0.0) {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = (c * g) + (s * y);
                x = -(s * g) + (c * y);
                for (int jj = 0; jj < N; ++jj) {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = (y * c) + (z * s);
                    a[jj][i] = -(y * s) + (z * c);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }  // its loop
    l3:;
    }
}

//----------------------------------------------------------------------------//

//
// a simple subroutine to compute the leading eigenvalue and eigenvector
// of a symmetric, traceless 4x4 matrix A by an inverse power iteration:
// (1) the matrix is converted to tridiagonal form by 3 Givens
// rotations;  V*A*V' = T
// (2) Gershgorin's theorem is used to estimate a lower
// bound for the leading negative eigenvalue:
// lambda_1 > g=min(T11-t12,-t21+T22-t23,-t32+T33-t34,-t43+T44)
//          =
// where tij=abs(Tij)
// (3) Form the positive definite matrix
//     B = T-gI
// (4) Use svd (algorithm svdcmp from "Numerical Recipes")
//     to compute eigenvalues and eigenvectors for SPD matrix B
// (5) Shift spectrum back and keep leading singular vector
//     and largest eigenvalue.
// (6) Convert eigenvector to original matrix A, through
//     multiplication by V'.
//

void dstmev(const double A[4][4], double& lambda, double evec[4]) {
    using std::fabs;
    using std::min;

    // (I) Convert to tridiagonal form, keeping similarity transform
    //            (a product of 3 Givens rotations)

    double T[4][4];
    double V[4][4];

    givens4(A, T, V);

    // (II) Estimate lower bound of smallest eigenvalue by Gershgorin's theorem
    lambda = T[0][0] - fabs(T[0][1]);
    lambda = min(lambda, T[1][1] - fabs(T[1][0]) - fabs(T[1][2]));
    lambda = min(lambda, T[2][2] - fabs(T[2][1]) - fabs(T[2][3]));
    lambda = min(lambda, T[3][3] - fabs(T[3][2]));

    // (III) Form positive definite matrix  T = lambda*I - T
    for (size_t i = 0; i < 4; ++i) T[i][i] -= lambda;

    // (IV) Compute singular values/vectors of SPD matrix B
    double SW[4], SV[4][4];
    svdcmp(T, SW, SV);

    // (V) Shift spectrum back
    int max_loc = 0;
    for (int i = 1; i < 4; ++i)
        if (SW[i] > SW[max_loc]) max_loc = i;

    lambda += SW[max_loc];

    // (VI) Convert eigenvector to original coordinates: (V is transposed!)
    for (int i = 0; i < 4; ++i) {
        evec[i] = 0.0;
        for (int j = 0; j < 4; ++j) evec[i] += V[i][j] * SV[j][max_loc];
    }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace rmsd_tools {

////////////////////////////////////////////////////////////////////////////////

double rmsd(size_t n, const double* RESTRICT w, double* RESTRICT x1, double* RESTRICT x2) {
    assert(n > 0);
    assert(w && x1 && x2);

    // find COMs

    double c1[3] = {0.0, 0.0, 0.0};
    double c2[3] = {0.0, 0.0, 0.0};

    double total_mass(0);

    for (size_t i = 0; i < n; ++i) {
        total_mass += w[i];
        const size_t i3 = 3 * i;
        for (size_t k = 0; k < 3; ++k) {
            c1[k] += w[i] * x1[i3 + k];
            c2[k] += w[i] * x2[i3 + k];
        }
    }

    for (size_t k = 0; k < 3; ++k) {
        c1[k] /= total_mass;
        c2[k] /= total_mass;
    }

    double x1n(0), x2n(0);

    for (size_t i = 0; i < n; ++i) {
        const size_t i3 = 3 * i;
        for (size_t k = 0; k < 3; ++k) {
            x1[i3 + k] -= c1[k];
            x1n += w[i] * x1[i3 + k] * x1[i3 + k];

            x2[i3 + k] -= c2[k];
            x2n += w[i] * x2[i3 + k] * x2[i3 + k];
        }
    }

    double lambda, q[4];
    rmsd_q(n, w, x1, x2, lambda, q);

    return std::sqrt(std::max(0.0, ((x1n + x2n) - 2 * lambda)) / total_mass);
}

//----------------------------------------------------------------------------//

void rmsd_q(size_t n, const double* RESTRICT w, const double* RESTRICT x1, const double* RESTRICT x2,
            double& RESTRICT lambda, double q[4]) {
    assert(n > 0);
    assert(w && x1 && x2);

    double R[3][3];

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j) R[i][j] = 0.0;

    for (size_t k = 0; k < n; ++k) {
        const size_t k3 = 3 * k;
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j) R[i][j] += w[k] * x1[k3 + i] * x2[k3 + j];
    }

    double S[4][4];

    S[0][0] = R[0][0] + R[1][1] + R[2][2];
    S[1][0] = R[1][2] - R[2][1];
    S[2][0] = R[2][0] - R[0][2];
    S[3][0] = R[0][1] - R[1][0];

    S[0][1] = S[1][0];
    S[1][1] = R[0][0] - R[1][1] - R[2][2];
    S[2][1] = R[0][1] + R[1][0];
    S[3][1] = R[0][2] + R[2][0];

    S[0][2] = S[2][0];
    S[1][2] = S[2][1];
    S[2][2] = -R[0][0] + R[1][1] - R[2][2];
    S[3][2] = R[1][2] + R[2][1];

    S[0][3] = S[3][0];
    S[1][3] = S[3][1];
    S[2][3] = S[3][2];
    S[3][3] = -R[0][0] - R[1][1] + R[2][2];

    dstmev(S, lambda, q);
}

//----------------------------------------------------------------------------//

void rmsd_q_to_rotation(const double* RESTRICT q, double* RESTRICT U) {
    assert(q && U);

    const double b0 = q[0] + q[0];
    const double b1 = q[1] + q[1];
    const double b2 = q[2] + q[2];
    const double b3 = q[3] + q[3];

    const double q00 = b0 * q[0] - 1.0;
    const double q01 = b0 * q[1];
    const double q02 = b0 * q[2];
    const double q03 = b0 * q[3];

    const double q11 = b1 * q[1];
    const double q12 = b1 * q[2];
    const double q13 = b1 * q[3];

    const double q22 = b2 * q[2];
    const double q23 = b2 * q[3];

    const double q33 = b3 * q[3];

#ifdef ROT
#undef ROT
#endif

#define ROT(i, j) U[3 * i + j]

    ROT(0, 0) = q00 + q11;
    ROT(1, 0) = q12 - q03;
    ROT(2, 0) = q13 + q02;

    ROT(0, 1) = q12 + q03;
    ROT(1, 1) = q00 + q22;
    ROT(2, 1) = q23 - q01;

    ROT(0, 2) = q13 - q02;
    ROT(1, 2) = q23 + q01;
    ROT(2, 2) = q00 + q33;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace rmsd_tools

////////////////////////////////////////////////////////////////////////////////
