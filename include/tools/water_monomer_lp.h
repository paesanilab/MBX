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

#ifndef WATER_MONOMER_LP_H
#define WATER_MONOMER_LP_H

/**
 * This structure sets up the positions of the lone pairs for water, and redistributes the gradients to the real sites
 * through chain rule.
 */
struct monomer {
    // Distance vector O-H1
    double oh1[3];
    // Distance vector O-H2
    double oh2[3];

    /**
     * @brief Sets up the positions of the 2 lone pairs
     *
     * See J. Chem. Theory Comput. 2013, 9, 5395−5403 for details (page 5396). Coordinates of LP are r+-L = ro + 1/2 *
     * gamma_parallel * (roh1 + roh2) +- gamma_perpendicular * (roh1 x roh2).
     * @param[in] ohh Pointer to coordinates of water, assuming OHH order
     * @param[in] in_plane_g Parameter that sets the relative position of the lone pairs. It is also known as w_parallel
     * @param[in] out_of_plane_g Parameter that sets the relative position of the lone pairs. It is also known as
     * w_perpendicular
     * @param[out] x1 Position of the first lone pair (xyz)
     * @param[out] x2 Position of the second lone pair (xyz)
     */
    void setup(const double* ohh, const double& in_plane_g, const double& out_of_plane_g, double x1[3], double x2[3]);

    /**
     * @brief Redistributes gradients in lone pairs to the real atoms
     *
     * Given the gradients in the two lone pairs, it will apply chain rule to redistribute them to the real atoms OHH
     * @param[in] g1 Gradients in xyz of the first lone pair.
     * @param[in] g2 Gradients in xyz of the second lone pair.
     * @param[in] in_plane_g Parameter that sets the relative position of the lone pairs. It is also known as w_parallel
     * @param[in] out_of_plane_g Parameter that sets the relative position of the lone pairs. It is also known as
     * w_perpendicular
     * @param[in,out] grd Pointer to the gradients of the water molecule assuming order OHH
     */
    void grads(const double* g1, const double* g2, const double& in_plane_g, const double& out_of_plane_g,
               double* grd) const;
};

#endif  // WATER_MONOMER_LP_H
