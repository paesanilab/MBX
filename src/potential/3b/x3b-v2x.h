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

#ifndef X3B_V2X_H
#define X3B_V2X_H

#include <vector>
#include <cmath>

#include "potential/3b/poly-3b-v2x.h"

/**
 * @file x3b-v2x.h
 * @brief Contains the structure of the polynomial holder for water-water-water MB-pol
 */

/**
 * @namespace x2o
 * @brief Encloses the structure of the polynomial holder for water-water-water MB-pol
 */
namespace x2o {

class x3b_v2x {
   public:
    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] w1 Pointer to a double array with the coordinates of the first water molecule.
     * @param[in] w2 Pointer to a double array with the coordinates of the second water molecule.
     * @param[in] w3 Pointer to a double array with the coordinates of the third water molecule.
     * @param[in] nt Number of trimers passed in the xyz arrays.
     * @return Double with the sum of the energies of each trimer.
     */
    static double eval(const double* w1, const double* w2, const double* w3, const size_t nt);

    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] w1 Pointer to a double array with the coordinates of the first water molecule.
     * @param[in] w2 Pointer to a double array with the coordinates of the second water molecule.
     * @param[in] w3 Pointer to a double array with the coordinates of the third water molecule.
     * @param[in] g1 Pointer to a double array with the gradients of the first water molecule.
     * @param[in] g2 Pointer to a double array with the gradients of the second water molecule.
     * @param[in] g3 Pointer to a double array with the gradients of the third water molecule.
     * @param[in] nt Number of trimers passed in the xyz arrays.
     * @param[in,out] virial Vector of 9 elements with the virial tensor.
     * @return Double with the sum of the energies of each trimer.
     */
    static double eval(const double* w1, const double* w2, const double* w3, double* g1, double* g2, double* g3,
                       const size_t nt, std::vector<double>* virial = 0);

    // Inner cutoff
    static const double r3i;

    // Outer cutoff
    static const double r3f;

   private:
    // Values of the non-linear parameters of the polynomials
    static const double kOH_intra;
    static const double kHH_intra;

    static const double kOO;
    static const double kOH;
    static const double kHH;

    static const double dOH_intra;
    static const double dHH_intra;

    static const double dOO;
    static const double dOH;
    static const double dHH;

    // Switch function
    static double f_switch(const double& r, double& g);
};

}  // namespace x2o

#endif  // X3B_V2X_H
