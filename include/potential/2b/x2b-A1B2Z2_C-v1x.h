/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#ifndef X2B_A1B2Z2_C_V1X_H
#define X2B_A1B2Z2_C_V1X_H

////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <string>
#include "poly-2b-A1B2Z2_C-v1x.h"

/**
 * @file x2b-A1B2Z2_C-v1x.h
 * @brief Contains the structure of the polynomial holder for symmetry A1B2Z2_C
 */

/**
 * @namespace h2o_ion
 * @brief Encloses the structure of the polynomial holder for symmetry A1B2Z2_C
 */
namespace h2o_ion {

class x2b_h2o_ion_v2x {
   public:
    // Creates an empty class
    x2b_h2o_ion_v2x(){};

    /**
     * @brief Creates a class and initializes the parameters corresponding to mon1 and mon2
     * @param[in] mon1 Monomer id of the first monomer of interest
     * @param[in] mon2 Monomer id of the second monomer of interest
     */
    x2b_h2o_ion_v2x(std::string mon1, std::string mon2);

    // Destroys the class
    ~x2b_h2o_ion_v2x(){};

    /**
     * @brief Computes the two body polynomials for the dimers
     *
     * Given the coordinates of a number of dimers (mon1 in xyz1 and mon2 in xyz2, it calculates the polynomial value
     * for each one of them, and returns the sum of the contributions for the dimers.
     * @param[in] w1 Pointer to a double array with the coordinates of the monomers of water.
     * @param[in] x Pointer to a double array with the coordinates of the monomers of monoatomic atom.
     * @param[in,out] g1 Pointer to a double array with the gradients of the monomers of waer.
     * @param[in,out] g2 Pointer to a double array with the gradients of the monomers of monoatomic atom.
     * @param[in] nd Number of dimers passed in the xyz arrays.
     * @param[in.out] virial Vector of doubles with the energies of each monomer, in the same order as the input.
     * @return Double with the sum of the energies of each dimer.
     */
    double eval(const double* w1, const double* x, double* g1, double* g2, const size_t nd,
                std::vector<double>* virial = 0);

    /**
     * @brief Computes the two body polynomials for the dimers
     *
     * Given the coordinates of a number of dimers (mon1 in xyz1 and mon2 in xyz2, it calculates the polynomial value
     * for each one of them, and returns the sum of the contributions for the dimers.
     * @param[in] w1 Pointer to a double array with the coordinates of the monomers of water.
     * @param[in] x Pointer to a double array with the coordinates of the monomers of the monoatomic atom.
     * @param[in] nd Number of dimers passed in the xyz arrays.
     * @return Double with the sum of the energies of each dimer.
     */
    double eval(const double* w1, const double* x, const size_t nd);

   private:
    // Values of the non-linear parameters of the polynomials
    double k_HH_intra;
    double k_OH_intra;

    double k_XH_coul;
    double k_XO_coul;

    double k_xlp_main;

    double d_HH_intra;
    double d_OH_intra;

    double d_XH_coul;
    double d_XO_coul;

    double d_xlp_main;

    double in_plane_gamma;
    double out_of_plane_gamma;

    // Inner cutoff
    double r2i;

    // Outer cutoff
    double r2f;

    // Switch function
    double f_switch(const double&, double&);

    // Values of the linear parameters of the polynomials
    std::vector<double> twobodyfit;
};

//----------------------------------------------------------------------------//

}  // namespace h2o_ion

////////////////////////////////////////////////////////////////////////////////

#endif  // X2B_H2O_ION_V2X_H
