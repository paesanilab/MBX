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

#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include <iostream>
#include "poly-3b-h2o-ion-v1x_deg4_filtered.h"

/**
 * @file x3b-h2o-ion-v1x-deg4-filtered.h
 * @brief Contains the structure of the polynomial holder for ion-water-water
 */

class x3b_h2o_ion_v1x_deg4_filtered {
   public:
    /**
     * @brief Creates a class and initializes the parameters corresponding to h2o, h2o and m3, which is the ion of
     * interest.
     * @param[in] m3 Monomer id of the ion of interest
     */
    x3b_h2o_ion_v1x_deg4_filtered(std::string m3);

    // Number of terms
    static const unsigned ncoeffs = h2o_ion::poly_3b_h2o_ion_v1x_deg4_filtered::size;

    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] xyz1 Pointer to a double array with the coordinates of the monomers of type mon1.
     * @param[in] xyz2 Pointer to a double array with the coordinates of the monomers of type mon2.
     * @param[in] xyz3 Pointer to a double array with the coordinates of the monomers of type mon3.
     * @param[in] nt Number of trimers passed in the xyz arrays.
     * @return Double with the sum of the energies of each trimer.
     */
    double operator()(const double* xyz1, const double* xyz2, const double* xyz3, size_t nt) const;

    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] xyz1 Pointer to a double array with the coordinates of the monomers of type mon1.
     * @param[in] xyz2 Pointer to a double array with the coordinates of the monomers of type mon2.
     * @param[in] xyz3 Pointer to a double array with the coordinates of the monomers of type mon3.
     * @param[in,out] grad1 Pointer to a double array with the gradients of the monomers of type mon1.
     * @param[in,out] grad2 Pointer to a double array with the gradients of the monomers of type mon2.
     * @param[in,out] grad3 Pointer to a double array with the gradients of the monomers of type mon3.
     * @param[in] nt Number of trimers passed in the xyz arrays.
     * @param[in,out] virial Vector of 9 elements with the virial tensor.
     * @return Double with the sum of the energies of each trimer.
     */
    double operator()(const double* xyz1, const double* xyz2, const double* xyz3, double* grad1, double* grad2,
                      double* grad3, size_t nt, std::vector<double>* virial = 0) const;

    // Inner cutoff
    double m_r3i;

    // Outer cutoff
    double m_r3f;

   private:
    // Values of the non-linear parameters of the polynomials
    double m_kOH_intra;
    double m_kHH_intra;

    double m_kOO;
    double m_kOH;
    double m_kHH;

    double m_kXO;
    double m_kXH;

    double m_dOH_intra;
    double m_dHH_intra;

    double m_dOO;
    double m_dOH;
    double m_dHH;

    double m_dXO;
    double m_dXH;

    // Values of the linear parameters of the polynomials
    double m_coeffs[ncoeffs];

    // Switch function
    double f_switch(const double& r, double& g) const;
};

//----------------------------------------------------------------------------//
