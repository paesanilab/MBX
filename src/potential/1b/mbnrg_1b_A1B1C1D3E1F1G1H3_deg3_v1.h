
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

#ifndef MBNRG_1B_A1B1C1D3E1F1G1H3_DEG3_V1_H
#define MBNRG_1B_A1B1C1D3E1F1G1H3_DEG3_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_1b_A1B1C1D3E1F1G1H3_deg3_v1.h" 


/**
 * @file mbnrg_1b_A1B1C1D3E1F1G1H3_deg3_v1.h
 * @brief Contains the structure of the polynomial holder for symmetry A1B1C1D3E1F1G1H3
 */

/**
 * @namespace mbnrg_A1B1C1D3E1F1G1H3_deg3
 * @brief Encloses the structure of the polynomial holder for symmetry A1B1C1D3E1F1G1H3
 */

namespace mbnrg_A1B1C1D3E1F1G1H3_deg3 {

//----------------------------------------------------------------------------//

struct mbnrg_A1B1C1D3E1F1G1H3_deg3_v1 {
    // Creates an empty class
    mbnrg_A1B1C1D3E1F1G1H3_deg3_v1() {};

    /**
     * @brief Creates a class and initializes the parameters corresponding to mon
     * @param[in] mon0 Monomer id of the first monomer of interest

     */
    mbnrg_A1B1C1D3E1F1G1H3_deg3_v1(const std::string mon1);

    // Destroys the class
    ~mbnrg_A1B1C1D3E1F1G1H3_deg3_v1() {};
    
    // Polynomial for this symmetry
    typedef poly_A1B1C1D3E1F1G1H3_deg3_v1 polynomial;

    /**
     * @brief Computes the one body energy for the monomers
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and ret
urns a vector with them.
     * @param[in] xyz0 Pointer to a double array with the coordinates of monomer 0 of the n-mer.
     
     * @param[in] n Number of monomers passed in the xyz array.
     * @return Vector of doubles with the energy.
     */
    std::vector<double> eval(const double *xyz1, const size_t n);

    /**
     * @brief Computes the one body energy for the monomers
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and ret
urns a vector with them.
     * @param[in] xyz0 Pointer to a double array with the coordinates of monomer 0 of the n-mer.
     * @param[out] grad0 Pointer to a double array with the gradients of monomer 0 of the n-mer.
    
     * @param[in] n Number of monomers passed in the xyz array.
     * @return Vector of doubles with the energy.
     */
    std::vector<double> eval(const double *xyz1, double *grad1 , const size_t n, std::vector<double> *virial=0);

  private:
    // Non-linear constants
    double m_k_x_intra_A_B_1;
    double m_k_x_intra_A_C_1;
    double m_k_x_intra_A_D_1;
    double m_k_x_intra_A_E_1;
    double m_k_x_intra_A_F_1;
    double m_k_x_intra_A_G_1;
    double m_k_x_intra_A_H_1;
    double m_k_x_intra_B_C_1;
    double m_k_x_intra_B_D_1;
    double m_k_x_intra_B_E_1;
    double m_k_x_intra_B_F_1;
    double m_k_x_intra_B_G_1;
    double m_k_x_intra_B_H_1;
    double m_k_x_intra_C_D_1;
    double m_k_x_intra_C_E_1;
    double m_k_x_intra_C_F_1;
    double m_k_x_intra_C_G_1;
    double m_k_x_intra_C_H_1;
    double m_k_x_intra_D_D_1;
    double m_k_x_intra_D_E_1;
    double m_k_x_intra_D_F_1;
    double m_k_x_intra_D_G_1;
    double m_k_x_intra_D_H_1;
    double m_k_x_intra_E_F_1;
    double m_k_x_intra_E_G_1;
    double m_k_x_intra_E_H_1;
    double m_k_x_intra_F_G_1;
    double m_k_x_intra_F_H_1;
    double m_k_x_intra_G_H_1;
    double m_k_x_intra_H_H_1;

    // Inner cutoff
    double m_ri = 7.0;
    
    // Outer cutoff
    double m_ro = 9.0;

    // Switch function
    double f_switch(const double, double&);

    // Vector with the coefficients of the polynomials
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

} // namespace mbnrg_A1B1C1D3E1F1G1H3_deg3

////////////////////////////////////////////////////////////////////////////////

#endif
