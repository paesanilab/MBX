/*
 * This is my own code to do harmonic vibrational frequencies
 * Currently reads a (non-mass-weighted) Hessian from disk.
 * Creates mass-weighted version.
 * Diagonalizes to produce normal modes.
 *
 * Update:
 * Now also includes code to generate localized modes
 * Based on Jacob and Reiher, JCP, 130, 084106 (2009)
 *
 */
/*********************************************************************/
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <time.h>  //for timers
#include "vib.h"
#include "nr.h"
#include <time.h>  //for timers
#include <cstdlib>
#include "io_tools/read_nrg.h"
#include "bblock/system.h"
using namespace std;
/*********************************************************************/
void jacobi_sweep(double* Q, double* mwH, double& L, double& max_change);
double K(double* Q, int s, int t, int u, int v);
void matsq(double* Min, double* Mout, const int dim);
/*********************************************************************/
#define pi 3.1415926535897932384626433832795
#define hbar 1.0e0
#define c_in_au 137.035999  //(a0/tau)
#define a0_to_cm 5.291772108e-9
/*********************************************************************/
// Global variables

// Input:

// User input:
int n;
int nwatermols;
double freqdiff;
double* eq;  // equilibrium geometry in angstrom
std::vector<bblock::System> systems;

/*********************************************************************/
int main(int argc, char* argv[]) {
    // Get input
    if ((argc - 1) != 3) {
        printf("Syntax: vib input.nrg mbx.json freqdiff (in cm^-1)\n");
        // printf("Syntax: vib <#atoms> <#water molecules> freqdiff (in cm^-1)\n");
        return 1;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> box;

    if (argc > 2) {
        systems[0].SetUpFromJson(argv[2]);
    } else {
        systems[0].SetUpFromJson();
    }

    n = int(systems[0].GetNumRealSites());
    std::vector<size_t> atom_tag;

    std::vector<std::string> at_names = systems[0].GetRealAtomNames();

    std::vector<std::string> mons = systems[0].GetMonId();
    std::vector<size_t> wat_indexes;
    nwatermols = 0;
    size_t current = 0;
    for (size_t i = 0; i < mons.size(); i++) {
        if (mons[i] == "h2o") {
            nwatermols++;
            wat_indexes.push_back(current);
        }
        current += systems[0].GetMonNumAt(i);
        for (size_t j = 0; j < systems[0].GetMonNumAt(i); j++) {
            atom_tag.push_back(i);
        }
    }

    freqdiff = atof(argv[3]);

    // 0.Setup
    double* H = AllocDouble(3 * n * 3 * n);        // bare Hessian
    double* mwH = AllocDouble(3 * n * 3 * n);      // mass-weighted Hessian
    double* mwHcopy = AllocDouble(3 * n * 3 * n);  // copy of mwH to be used later (diagon destroys)
    double* m = AllocDouble(n);                    // masses in a.u.
    ifstream indata;
    indata.open("mass.dat");  // masses in amu (of the ATOMS)

    if (!indata) {
        printf("Error: mass.dat could not be opened.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) indata >> m[i];

    // XC print out mass
    printf("input mass:\n");

    for (int i = 0; i < n; i++) printf(" % -10.8f\n", m[i]);
    printf("\n");
    // end XC

    for (int i = 0; i < n; i++) m[i] *= 1822.8883942;
    indata.close();

    // 1.Read non-mass-weighted Hessian from disk
    indata.open("hess_au.dat");
    if (!indata) {
        printf("Error: hess.dat could not be opened.\n");
        exit(1);
    }
    for (int i = 0; i < (3 * n * 3 * n); i++) indata >> H[i];
    indata.close();

    // Read equilibrium geometry
    eq = AllocDouble(3 * n);
    indata.open("geom_ang.dat");
    if (!indata) {
        printf("Error: geom_ang.dat could not be opened.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) indata >> eq[i * 3 + j];
    }
    indata.close();

    // print out eq geometry
    printf("equilibrium geometry :\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) printf("% -15.8f  ", eq[i * 3 + j]);
        printf("\n");
    }

    // 2.Create mass-weighted Hessian
    for (int i = 0; i < n; i++)
        for (int di = 0; di < 3; di++)
            for (int j = 0; j < n; j++)
                for (int dj = 0; dj < 3; dj++)
                    mwH[(i * 3 + di) * (3 * n) + (j * 3 + dj)] =
                        1.0 / sqrt(m[i] * m[j]) * H[(i * 3 + di) * (3 * n) + (j * 3 + dj)];

    for (int i = 0; i < 3 * n * 3 * n; i++) mwHcopy[i] = mwH[i];

    printf("\n");

    printf("Original mass-weighted Hessian:\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) {
            printf(" % -10.8f", mwH[i * (3 * n) + j]);
        }
        printf("\n");
    }

    // 3.Diagonalize
    double* U = AllocDouble(3 * n * 3 * n);  // evec matrix (normal modes)
    double* V = AllocDouble(3 * n);          // eval vector (4*pi^2*w^2)
    diagon(mwH, V, U, 3 * n);

    printf("U(normal modes matrix):\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) printf("% -4.3f  ", U[i * 3 * n + j]);
        printf("\n");
    }

    // 4.Compute harmonic frequencies
    printf("angular frequencies (a.u.):\n");
    // for(int i=0 ; i<3*n ; i++)
    for (int i = 0; i < 3 * n - 6; i++) printf(" % -10.8f\n", sqrt(V[i]));

    printf("frequencies (cm-1):\n");
    // for(int i=0 ; i<3*n ; i++)
    for (int i = 0; i < 3 * n - 6; i++) printf(" % -10.8f\n", sqrt(V[i]) / (2.0 * pi) / c_in_au / a0_to_cm);
    printf("low frequencies (cm-1):\n");
    for (int i = 3 * n - 6; i < 3 * n; i++) printf(" % -10.8f\n", sqrt(V[i]) / (2.0 * pi) / c_in_au / a0_to_cm);

    // 5.Create normal modes
    printf("Mass-weighted Normal mode displacements (a.u.):\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    {
        printf("Mode %i\n", i + 1);
        printf("    x       y      z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -4.3f ", U[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }

    // Added: normalize the mass-weighted cartesian displacements
    printf("U[0] = % -10.8f\n", U[0]);
    double* mnorm = AllocDouble(3 * n);
    double* mU = AllocDouble(3 * n * 3 * n);
    for (int i = 0; i < 3 * n * 3 * n; i++) mU[i] = U[i];
    for (int i = 0; i < 3 * n; i++) {
        mnorm[i] = 0.0;
        for (int j = 0; j < 3 * n; j++) mnorm[i] += mU[j * 3 * n + i] * mU[j * 3 * n + i];
    }

    // Testing: Reduced mass should be U^t[M^-2]U
    // Where M is defined as 1/sqrt(m) matrix (diagonal)
    double* M = AllocDouble(3 * n * 3 * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < n; k++) {
                for (int l = 0; l < 3; l++) {
                    if (i == k && j == l)
                        M[(3 * i + j) * (3 * n) + (k * 3 + l)] = 1.0 / (m[i]);  // M^-2 (since M is diagonal)
                    else
                        M[(3 * i + j) * (3 * n) + (k * 3 + l)] = 0.0;
                }
            }
        }
    }

    double* MU = AllocDouble(3 * n * 3 * n);
    matmultAB(M, U, MU, 3 * n);
    double* UMU = AllocDouble(3 * n * 3 * n);
    matmultAtB(U, MU, UMU, 3 * n);

    printf("reduced mass:\n");
    for (int i = 0; i < 3 * n - 6; i++) {
        {
            printf(" % -10.2f", 1 / UMU[i * 3 * n + i]);
        }
        printf("\n");
    }

    free(M);
    free(MU);
    // free(UMU);
    // End Testing

    // Un-mass-weight for Cartesian displacements
    double* unmwU = AllocDouble(3 * n * 3 * n);
    for (int i = 0; i < 3 * n; i++)  // loop over modes
        for (int j = 0; j < n; j++)  // atomic contributions to mode
            for (int d = 0; d < 3; d++)
                unmwU[(j * 3 + d) * (3 * n) + i] = 1.0 / sqrt(m[j]) * U[(j * 3 + d) * (3 * n) + i];
    printf("Cartesian Normal mode displacements (a.u.):\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    {
        printf("Mode %i\n", i + 1);
        printf("    x       y      z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -4.3f ", unmwU[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }

    // Normalize the Cartesian displacements
    double* norm = AllocDouble(3 * n);
    for (int i = 0; i < 3 * n; i++) {
        norm[i] = 0.0;
        for (int j = 0; j < 3 * n; j++) norm[i] += unmwU[j * 3 * n + i] * unmwU[j * 3 * n + i];
    }

    for (int i = 0; i < 3 * n; i++)  // loop over vib modes, norm for each
        for (int j = 0; j < 3 * n; j++) unmwU[j * 3 * n + i] /= sqrt(norm[i]);
    printf("[normalized] Cartesian Normal mode displacements (a.u.):\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    {
        printf("Mode %i\n", i + 1);
        printf(" x   y   z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -10.8f ", unmwU[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }

    free(unmwU);

    // Local mode code-------------------------------------------------

    // Notation
    // L = locality measure
    // Q = normal modes
    // double* Q = U;
    double* Q = AllocDouble(3 * n * 3 * n);

    for (int i = 0; i < 3 * n * 3 * n; i++) Q[i] = U[i];
    // Normalize the mass-weighted Cartesian displacements
    for (int i = 0; i < 3 * n; i++) {
        norm[i] = 0.0;
        for (int j = 0; j < 3 * n; j++) norm[i] += Q[j * 3 * n + i] * Q[j * 3 * n + i];
    }
    for (int i = 0; i < 3 * n; i++)  // loop over vib modes, norm for each
        for (int j = 0; j < 3 * n; j++) Q[j * 3 * n + i] /= sqrt(norm[i]);

    double L;
    double temp;

    // 1.Generate starting L
    L = 0.0;
    // for(int r=0 ; r<3*n ; r++) //all modes (eqn B9)
    for (int r = 0; r < 3 * n - 6; r++)  // all modes (eqn B9)
        L += K(Q, r, r, r, r);

    // 2.Jacobi sweeps
    int maxsweeps = 10000;
    int converged = 0;
    double thresh = 1.0e-14;
    double max_change;
    double testL;

    mwH = mwHcopy;

    for (int iswp = 0; iswp < maxsweeps; iswp++) {
        printf(" Sweep %3i : L=% -10.8f  max_change=% -10.8e\n", iswp, L, max_change);
        if (converged != 1) {
            jacobi_sweep(Q, mwH, L, max_change);

            if (max_change < thresh) {
                converged = 1;
                printf("Jacobi sweeps converged in %3i steps\n", iswp);
                break;
            }

            if (iswp == (maxsweeps - 1)) {
                printf("Error: Jacobi sweeps did not converge.\n");
                exit(1);
            }
        }
    }

    // End local mode code--------------------------------------------------

    printf("TEMP: Q upon localization:\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) {
            printf(" % -10.8f", Q[i * (3 * n) + j]);
        }
        printf("\n");
    }
    // Look at new modes:

    // 5.Create Local normal modes
    printf("Normalized, Mass-weighted Local Normal mode displacements (a.u.):\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    // for(int i=(3*n-7) ;i>=0 ; i--) //Fix order to match q-chem
    {
        printf("Mode %i\n", i + 1);
        printf("    x       y      z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -15.8f ", Q[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }

    // Un-mass-weight for Cartesian displacements
    unmwU = AllocDouble(3 * n * 3 * n);
    for (int i = 0; i < 3 * n; i++)  // loop over modes
        for (int j = 0; j < n; j++)  // atomic contributions to mode
            for (int d = 0; d < 3; d++)
                unmwU[(j * 3 + d) * (3 * n) + i] = 1.0 / sqrt(m[j]) * Q[(j * 3 + d) * (3 * n) + i];
    printf("Cartesian Local Normal mode displacements (a.u.):\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    {
        printf("Mode %i\n", i + 1);
        printf("    x       y      z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -4.3f ", unmwU[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }
    // Normalize the Cartesian displacements
    double* normQ = AllocDouble(3 * n);
    for (int i = 0; i < 3 * n; i++) {
        normQ[i] = 0.0;
        for (int j = 0; j < 3 * n; j++) normQ[i] += unmwU[j * 3 * n + i] * unmwU[j * 3 * n + i];
    }
    // temp test

    std::ofstream offrm("redmass.dat");
    printf("1/normQ is reduced mass of local modes (m_e): \n");
    // for(int i=0 ; i<3*n ; i++)
    for (int i = 0; i < 3 * n - 6; i++)  // XC
    {
        // printf("1/normQ[%3i] = % -10.8f (m_e)\n",i,1.0/normQ[i]);
        if (i < 3 * nwatermols) {
            offrm << std::setprecision(8) << std::fixed << std::setw(15) << 1.0 / normQ[i] << std::endl;
        }
        printf(" % -15.8f \n", 1.0 / normQ[i]);  // XC
    }
    offrm.close();

    // end test
    for (int i = 0; i < 3 * n; i++)  // loop over vib modes, norm for each
        for (int j = 0; j < 3 * n; j++) unmwU[j * 3 * n + i] /= sqrt(normQ[i]);
    printf("[normalized] Cartesian Local mode displacements (a.u.):\n");
    std::ofstream off("modes.dat");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    // for(int i=(3*n-7) ;i>=0 ; i--) //Fix order to match q-chem
    {
        printf("Mode %i\n", i + 1);
        printf(" x   y   z\n");
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) {
                printf(" % -15.8f ", unmwU[(j * 3 + d) * (3 * n) + i]);
                off << std::setprecision(8) << std::fixed << std::setw(15) << unmwU[(j * 3 + d) * (3 * n) + i];
            }
            off << std::endl;
            printf("\n");
        }
    }
    off.close();

    // Determine which modes belong to which monomer
    int nmaxx = 0;
    double tempp = 0.0;

    printf("mode     monomer \n");

    std::vector<int> modexxx;
    std::vector<int> monxxx;
    // i is atom index.
    // we wanna loop over atoms of water molecules
    // for (int i = 0; i < 3 * nwatermols; i++) {
    // FIXME assuming that the water modes are always the first ones
    for (size_t i = 0; i < 3 * nwatermols; i++) {
        tempp = 0.0;
        nmaxx = 0;
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++)
                if (abs(unmwU[(j * 3 + d) * (3 * n) + i]) > tempp) {
                    tempp = abs(unmwU[(j * 3 + d) * (3 * n) + i]);
                    // nmaxx = j / 3 + 1;
                    nmaxx = atom_tag[j] + 1;
                }
        }
        printf("%i   %i\n ", i + 1, nmaxx);
        monxxx.push_back(nmaxx);
        modexxx.push_back(i + 1);
    }
    std::ofstream offass("assign.dat");
    size_t minmon = 1000;
    while (monxxx.size() > 0) {
        for (size_t kk = 0; kk < monxxx.size(); kk++) {
            if (minmon > monxxx[kk]) minmon = monxxx[kk];
        }

        for (size_t kk = 0; kk < monxxx.size(); kk++) {
            if (monxxx[kk] == minmon) {
                offass << std::setw(5) << modexxx[kk] << std::setw(5) << monxxx[kk] << std::endl;
                modexxx.erase(modexxx.begin() + kk);
                monxxx.erase(monxxx.begin() + kk);
                kk--;
            }
        }
        minmon++;
        if (monxxx.size() < 1) break;
    }
    offass.close();

    printf("[normalized] Cartesian Local Normal mode displacements (a.u.) [q-chem format]:\n");
    int nloops = n - 2;  //(3n-6)/3
    for (int i = 0; i < nloops; i++) {
        printf("Mode %i       Mode %i       Mode %i\n", (3 * i + 0) + 1, (3 * i + 1) + 1, (3 * i + 2) + 1);
        printf("               X      Y      Z        X      Y      Z        X      Y      Z\n");
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 3; k++) {
                // XC
                // int mode=(3*n-7)-i*3-k;
                int mode = i * 3 + k;
                // end XC

                for (int d = 0; d < 3; d++) {
                    if (k == 0 && d == 0) {
                        printf("% 11.5s   % -4.3f ", at_names[j], unmwU[(j * 3 + d) * (3 * n) + mode]);
                    } else
                        printf("% -4.3f ", unmwU[(j * 3 + d) * (3 * n) + mode]);
                }
                printf("  ");
            }
            printf("\n");
        }
    }

    printf("Old Molden version:\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot
    {
        printf("vibration %i\n", i + 1);
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -4.3f ", unmwU[(j * 3 + d) * (3 * n) + i]);
            printf("\n");
        }
    }
    //    free(unmwU);

    printf("Molden version:\n");
    for (int i = 0; i < (3 * n - 6); i++)  // exclude trans/rot, also plot in rev order to match energy ordering
    {
        int revi = 3 * n - 7 - i;
        // printf("revi=%i\n",revi);
        printf("vibration %i\n", i + 1);
        for (int j = 0; j < n; j++) {
            for (int d = 0; d < 3; d++) printf(" % -16.8f ", unmwU[(j * 3 + d) * (3 * n) + i]);
            // printf(" % -4.3f ",unmwU[(j*3+d)*(3*n)+revi]);
            printf("\n");
        }
    }

    // Original diagonal Hessian, now rotated into local-mode basis
    // Note: [Q^t Horig Q] = Hnorm
    //      L = QU (local modes)
    //      Therefore, what we seek is U^t Hnorm U = U^t [Q^t Horig Q] U = (QU)^t Horig (QU) = L^t Horig L
    //      Notation in this code: "Q" at this point is QU

    // First, "unnormalize" Q <--Needed???
    for (int i = 0; i < 3 * n; i++)  // loop over vib modes, UNnorm for each
        for (int j = 0; j < 3 * n; j++) Q[j * 3 * n + i] *= sqrt(norm[i]);

    // Forming Q^t H Q
    double* temp1 = AllocDouble(3 * n * 3 * n);
    double* locH = AllocDouble(3 * n * 3 * n);
    mwH = mwHcopy;
    matmultAB(mwH, Q, temp1, 3 * n);
    matmultAtB(Q, temp1, locH, 3 * n);
    // procedural testing only:
    // matmultAB(mwH,U,temp1,3*n);
    // matmultAtB(U,temp1,locH,3*n);

    // Print the three different Hessians
    printf("Original mass-weighted Cartesian Hessian:\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) {
            printf(" % -10.8f", mwH[i * (3 * n) + j]);
        }
        printf("\n");
    }
    printf("Pseudo-frequencies of original mass-weighted Cartesian Hessian:\n");
    for (int i = 0; i < 3 * n; i++) {
        if (fabs(mwH[i * (3 * n) + i]) < 1.0e-15)
            printf(" zero freq\n", sqrt(mwH[i * (3 * n) + i]) / (2.0 * pi) / c_in_au / a0_to_cm);
        else
            printf(" % -10.8f\n", sqrt(mwH[i * (3 * n) + i]) / (2.0 * pi) / c_in_au / a0_to_cm);
    }
    printf("Diagonal elts of diagonalized mass-weighted Cartesian Hessian:\n");
    for (int i = 0; i < 3 * n; i++) printf(" % -10.8f\n", V[i]);
    printf("Local-mode-transformed mass-weighted Cartesian Hessian:\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) {
            printf(" % -10.8f", locH[i * (3 * n) + j]);
        }
        printf("\n");
    }
    printf("Local-mode-transformed mass-weighted Cartesian Hessian (mode block only, more detail):\n");
    for (int i = 0; i < 3 * n - 6; i++) {
        for (int j = 0; j < 3 * n - 6; j++) {
            printf(" % -16.15f", locH[i * (3 * n) + j]);
        }
        printf("\n");
    }

    printf(
        "Local-mode-transformed mass-weighted Cartesian Hessian (mode block only, written as w^2 matrix in "
        "[cm-1]^2):\n");
    for (int i = 0; i < 3 * n - 6; i++) {
        for (int j = 0; j < 3 * n - 6; j++) {
            // printf(" % -16.15f",locH[i*(3*n)+j]*219474.6313);
            printf(" % -16.8f", locH[i * (3 * n) + j] * 219474.6313 * 219474.6313);  // XC
        }
        printf("\n");
    }

    printf("Local-mode-transformed mass-weighted Cartesian Hessian (mode block only, written as w matrix in cm-1):\n");
    for (int i = 0; i < 3 * n - 6; i++) {
        for (int j = 0; j < 3 * n - 6; j++) {
            // printf(" % -16.15f",sqrt(locH[i*(3*n)+j])*219474.6313);
            printf(" % -16.8f", sqrt(fabs(locH[i * (3 * n) + j])) * 219474.6313);  // XC
        }
        printf("\n");
    }

    std::ofstream offpf("omega.dat");
    printf("Pseudo-frequencies of local-mode-transformed Hessian:\n");
    for (int i = 0; i < 3 * n; i++) {
        if (fabs(locH[i * (3 * n) + i]) < 1.0e-15)
            printf(" zero freq\n", sqrt(locH[i * (3 * n) + i]) / (2.0 * pi) / c_in_au / a0_to_cm);
        else
            printf(" % -15.8f\n", sqrt(locH[i * (3 * n) + i]) / (2.0 * pi) / c_in_au / a0_to_cm);
        if (i < 3 * nwatermols) {  // FIXME extrahardcoded for water
            offpf << std::fixed << std::setprecision(8) << std::setw(15)
                  << sqrt(locH[i * (3 * n) + i]) / (2.0 * pi) / c_in_au / a0_to_cm << std::endl;
        }
    }
    offpf.close();

    printf("vibrational frequencies of the local modes: (cm^-1) [q-chem format]:\n");

    for (int i = 0; i < nloops; i++) {
        printf(" Mode:               %3i                    %3i                    %3i\n", (3 * i + 0) + 1,
               (3 * i + 1) + 1, (3 * i + 2) + 1);

        printf(" Frequency:      %-4.2f                %-4.2f                %-4.2f\n",
               sqrt(locH[3 * n * (3 * i) + (3 * i)]) / (2.0 * pi) / c_in_au / a0_to_cm,
               sqrt(locH[3 * n * (3 * i + 1) + (3 * i + 1)]) / (2.0 * pi) / c_in_au / a0_to_cm,
               sqrt(locH[3 * n * (3 * i + 2) + (3 * i + 2)]) / (2.0 * pi) / c_in_au / a0_to_cm);

        printf(" Force Cnst:     %-5.4f                %-5.4f                %-5.4f\n", 1.1253, 1.1253, 1.1253);

        printf(" Red. Mass:      %-5.4f                %-5.4f                %-5.4f\n",
               1.0 / normQ[3 * i] / 1822.8883942, 1.0 / normQ[3 * i + 1] / 1822.8883942,
               1.0 / normQ[3 * i + 2] / 1822.8883942);

        printf(" IR Active:       YES                   YES                   YES\n");

        printf(" IR Intens:        %-4.3f               %-4.3f                %-4.3f\n", 1.292, 1.292, 1.292);

        printf(" Raman Active:    YES                   YES                   YES\n");

        printf("                X      Y      Z        X      Y      Z        X      Y      Z\n");
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 3; k++) {
                // XC
                // int mode=(3*n-7)-i*3-k;
                int mode = i * 3 + k;
                // end XC

                for (int d = 0; d < 3; d++) {
                    if (k == 0 && d == 0) {
                        printf("% 11.5s  % -4.3f ", at_names[j], unmwU[(j * 3 + d) * (3 * n) + mode]);
                    } else
                        printf("% -4.3f ", unmwU[(j * 3 + d) * (3 * n) + mode]);
                }
                printf("  ");
            }
            printf("\n");
        }
        printf(" TransDip   %-4.3f %-4.3f %-4.3f %-4.3f %-4.3f %-4.3f %-4.3f %-4.3f %-4.3f\n", 0.0, 0.0, 0.0, 0.0, 0.0,
               0.0, 0.0, 0.0, 0.0);
        printf(" ");
        printf("\n");
    }

    // Generate local-mode transformed Cartesian (un-mass-weighted) Hessian for FChk files
    double* loccartH = AllocDouble(3 * n * 3 * n);
    for (int i = 0; i < 3 * n * 3 * n; i++) loccartH[i] = H[i];
    double* loctemp = AllocDouble(3 * n * 3 * n);
    matmultAB(loccartH, Q, loctemp, 3 * n);
    matmultAtB(Q, loctemp, loccartH, 3 * n);
    free(loctemp);
    printf("Cartesian Hessian, transformed to local:\n");
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 3 * n; j++) {
            printf(" % -10.8f", loccartH[i * 3 * n + j]);
        }
        printf("\n");
    }
    printf(" ...FChk version:\n");  // Only prints lower triangle, in five columns
    int count = 0;
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j <= i; j++) {
            printf(" % -9.8e", loccartH[i * 3 * n + j]);
            count++;
            if (count == 5) {
                printf("\n");
                count = 0;
            }
        }
    }

    free(U);
    free(V);
    free(mwH);
    free(unmwU);
    free(locH);

    return 0;
}

/************************************************/
void jacobi_sweep(double* Q, double* mwH, double& L, double& max_change) {
    max_change = 0.0;
    double A, B;
    double alpha1, alpha2, alpha;
    double den;
    double change;
    double sa, ca, epsilon1, epsilon2;
    double* Cp = AllocDouble(3 * n);
    double* Cq = AllocDouble(3 * n);
    max_change = 0.0;  // reset
    int p_max_change, q_max_change;

    // XC
    double pfreq, qfreq;

    for (int p = 0; p < 3 * n - 6; p++) {
        for (int q = p + 1; q < 3 * n - 6; q++) {
            // XC: rotate only if pfreq and qfreq are close

            pfreq = 0.0;
            for (int j = 0; j < 3 * n; j++) {
                for (int k = 0; k < 3 * n; k++) pfreq += Q[j * 3 * n + p] * mwH[j * 3 * n + k] * Q[k * 3 * n + p];
            }
            pfreq = sqrt(pfreq) / (2.0 * pi) / c_in_au / a0_to_cm;

            qfreq = 0.0;
            for (int j = 0; j < 3 * n; j++) {
                for (int k = 0; k < 3 * n; k++) qfreq += Q[j * 3 * n + q] * mwH[j * 3 * n + k] * Q[k * 3 * n + q];
            }
            qfreq = sqrt(qfreq) / (2.0 * pi) / c_in_au / a0_to_cm;

            if (fabs(pfreq - qfreq) < freqdiff)  // XC: difference of frequency (cm^-1)
            {
                // END XC

                // Build A,B matrices
                A = K(Q, p, q, p, q) - 0.25 * (K(Q, p, p, p, p) + K(Q, q, q, q, q) - 2.0 * K(Q, p, p, q, q));  //(B10)
                B = K(Q, p, p, p, q) - K(Q, q, q, q, p);                                                       //(B11)

                if (fabs(B) > 1.0e-12) {
                    // Determine alpha
                    alpha = atan(-B / A);  // returns on [-pi/2,pi/2]
                                           // But if -1<-B/A<1, then returns on [-pi/4,pi/4]

                    if (alpha < 0.0 && B > 0.0)  // move 4th --> 2nd quadrant
                        alpha += pi;
                    else if (alpha > 0.0 && B < 0.0)  // move 1st --> 3rd quadrant
                        alpha -= pi;
                    alpha /= 4.0;

                    // Update L
                    change = A + sqrt(A * A + B * B);  //(B14)
                    if (change > max_change) {
                        max_change = change;
                        p_max_change = p;
                        q_max_change = q;
                    }
                    L += change;

                    // Update Q using alpha
                    //(Since K is generated on-the-fly, we needn't update it.)
                    sa = sin(alpha);
                    ca = cos(alpha);
                    // Rotation matrix only affects columns p,q of Q
                    // newQrp = cQrp - sQrq
                    // newQrq = sQrp + cQrq
                    // Vectors here are new columns
                    for (int r = 0; r < 3 * n; r++) {
                        Cp[r] = ca * Q[r * 3 * n + p] + sa * Q[r * 3 * n + q];
                        Cq[r] = -sa * Q[r * 3 * n + p] + ca * Q[r * 3 * n + q];
                    }
                    for (int r = 0; r < 3 * n; r++) {
                        Q[r * 3 * n + p] = Cp[r];
                        Q[r * 3 * n + q] = Cq[r];
                    }
                }
            }
        }
    }

    free(Cp);
    free(Cq);
}

/************************************************/
// Computes a single element of the K tensor, K_stuv
// Builds from normal mode input Q
// Eqn B6
double K(double* Q, int s, int t, int u, int v) {
    double val = 0.0;
    double val1, val2, temp1, temp2;

    for (int dd = 0; dd < 3; dd++) {
        temp1 = 0.0;
        temp2 = 0.0;

        for (int i = 0; i < n; i++) {
            val1 = 0.0;
            val2 = 0.0;
            for (int d = 0; d < 3; d++) {
                val1 += Q[(3 * i + d) * 3 * n + s] * Q[(3 * i + d) * 3 * n + t];
                val2 += Q[(3 * i + d) * 3 * n + u] * Q[(3 * i + d) * 3 * n + v];
            }
            val1 *= eq[i * 3 + dd];
            val2 *= eq[i * 3 + dd];
            temp1 += val1;
            temp2 += val2;
        }
        val += temp1 * temp2;
    }
    return val;
}
/************************************************/
// computes the square of a matrix
void matsq(double* Min, double* Mout, const int dim) {
    int N = dim;
    for (int i = 0; i < N * N; i++) Mout[i] = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++) Mout[i * N + j] += Min[i * N + k] * Min[j * N + k];
}
