
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

#ifndef MBNRG_3B_A1_B1C2X2_B1C2X2_DEG4_V2_H
#define MBNRG_3B_A1_B1C2X2_B1C2X2_DEG4_V2_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1_B1C2X2_B1C2X2_deg4_v2.h" 


/**
 * @file mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v2.h
 * @brief Contains the structure of the polynomial holder for symmetry A1_B1C2X2_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2
 * @brief Encloses the structure of the polynomial holder for symmetry A1_B1C2X2_B1C2X2
 */

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2 {

//----------------------------------------------------------------------------//

struct mbnrg_A1_B1C2X2_B1C2X2_deg4_v2 {
    // Creates an empty class
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v2() {};

    /**
     * @brief Creates a class and initializes the parameters corresponding to mon
     * @param[in] mon0 Monomer id of the first monomer of interest
     * @param[in] mon1 Monomer id of the first monomer of interest
     * @param[in] mon2 Monomer id of the first monomer of interest

     */
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v2(const std::string mon1, const std::string mon2, const std::string mon3);

    // Destroys the class
    ~mbnrg_A1_B1C2X2_B1C2X2_deg4_v2() {};
    
    // Polynomial for this symmetry
    typedef poly_A1_B1C2X2_B1C2X2_deg4_v2 polynomial;

    /**
     * @brief Computes the one body energy for the monomers
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and ret
urns a vector with them.
     * @param[in] xyz0 Pointer to a double array with the coordinates of monomer 0 of the n-mer.
     * @param[in] xyz1 Pointer to a double array with the coordinates of monomer 1 of the n-mer.
     * @param[in] xyz2 Pointer to a double array with the coordinates of monomer 2 of the n-mer.
     
     * @param[in] n Number of monomers passed in the xyz array.
     * @return Double with the energy.
     */
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);

    /**
     * @brief Computes the one body energy for the monomers
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and ret
urns a vector with them.
     * @param[in] xyz0 Pointer to a double array with the coordinates of monomer 0 of the n-mer.
     * @param[in] xyz1 Pointer to a double array with the coordinates of monomer 1 of the n-mer.
     * @param[in] xyz2 Pointer to a double array with the coordinates of monomer 2 of the n-mer.
     * @param[out] grad0 Pointer to a double array with the gradients of monomer 0 of the n-mer.
     * @param[out] grad1 Pointer to a double array with the gradients of monomer 1 of the n-mer.
     * @param[out] grad2 Pointer to a double array with the gradients of monomer 2 of the n-mer.
    
     * @param[in] n Number of monomers passed in the xyz array.
     * @return Double with the energy.
     */
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3 , const size_t n, std::vector<double> *virial=0);

  private:
    // Non-linear constants
    double m_k_x_inter_A_B_0;
    double m_k_x_inter_A_C_0;
    double m_k_x_inter_A_X_0;
    double m_k_x_inter_B_B_0;
    double m_k_x_intra_B_C_1;
    double m_k_x_inter_B_C_0;
    double m_k_x_inter_B_X_0;
    double m_k_x_intra_C_C_1;
    double m_k_x_inter_C_C_0;
    double m_k_x_inter_C_X_0;
    double m_k_x_inter_X_X_0;

    // Inner cutoff
    double m_ri = 0.0;
    
    // Outer cutoff
    double m_ro = 6.0;

    // Switch function
    double f_switch(const double, double&);

    // Vector with the coefficients of the polynomials
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

} // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2

////////////////////////////////////////////////////////////////////////////////

#endif
