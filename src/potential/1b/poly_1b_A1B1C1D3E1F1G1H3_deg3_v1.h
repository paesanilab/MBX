
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



#ifndef POLY_1B_MBNRG_A1B1C1D3E1F1G1H3_DEG3_H
#define POLY_1B_MBNRG_A1B1C1D3E1F1G1H3_DEG3_H

/**
 * @file poly_1b_A1B1C1D3E1F1G1H3_deg3_v1.h
 * @brief Contains the structure of the polynomial for symmetry A1B1C1D3E1F1G1H3
 */

/**
 * @namespace mbnrg_A1B1C1D3E1F1G1H3_deg3
 * @brief Encloses the structure of the polynomial for symmetry A1B1C1D3E1F1G1H3
 */

namespace mbnrg_A1B1C1D3E1F1G1H3_deg3 {

struct poly_A1B1C1D3E1F1G1H3_deg3_v1 {
    // Degree of the polynomial
    static const unsigned degree = 3;

    // Number of variables
    static const unsigned n_vars = 66;

    // Number of terms
    static const unsigned size = 8039;

    /**
     * @brief Evaluates the polynomial of degree 3 for A1B1C1D3E1F1G1H3 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1B1C1D3E1F1G1H3 symmetry.
     * @param[in] x Double array of length 66 with the variable values
     * @param[in] a Double array of 8039 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval(const double x[66],
              const double a[8039]);

    /**
     * @brief Evaluates the polynomial of degree 3 for A1B1C1D3E1F1G1H3 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1B1C1D3E1F1G1H3 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 66 with the variable values
     * @param[in] a Double array of 8039 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval_direct(const double x[66],
                     const double a[8039]);

    /**
     * @brief Evaluates the polynomial of degree 3 for A1B1C1D3E1F1G1H3 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1B1C1D3E1F1G1H3 symmetry.
     * @param[in] x Double array of length 66 with the variable values
     * @param[in] a Double array of 8039 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 66 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval(const double x[66],
              const double a[8039],
                    double g[66]);
    
    /**
     * @brief Evaluates the polynomial of degree 3 for A1B1C1D3E1F1G1H3 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1B1C1D3E1F1G1H3 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 66 with the variable values
     * @param[in] a Double array of 8039 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 66 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval_direct(const double x[66],
                     const double a[8039],
                           double g[66]);
};

} // namespace mbnrg_A1B1C1D3E1F1G1H3_deg3

#endif // POLY_1B_MBNRG_A1B1C1D3E1F1G1H3_DEG3_H



//Polynomial input used to generate these files:

//  add_molecule['A1B1C1D3E1F1G1H3']
//  
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'C', '1', 'a', 'x-intra-A+C-1']
//  add_variable['A', '1', 'a', 'D', '1', 'a', 'x-intra-A+D-1']
//  add_variable['A', '1', 'a', 'D', '2', 'a', 'x-intra-A+D-1']
//  add_variable['A', '1', 'a', 'D', '3', 'a', 'x-intra-A+D-1']
//  add_variable['A', '1', 'a', 'E', '1', 'a', 'x-intra-A+E-1']
//  add_variable['A', '1', 'a', 'F', '1', 'a', 'x-intra-A+F-1']
//  add_variable['A', '1', 'a', 'G', '1', 'a', 'x-intra-A+G-1']
//  add_variable['A', '1', 'a', 'H', '1', 'a', 'x-intra-A+H-1']
//  add_variable['A', '1', 'a', 'H', '2', 'a', 'x-intra-A+H-1']
//  add_variable['A', '1', 'a', 'H', '3', 'a', 'x-intra-A+H-1']
//  add_variable['B', '1', 'a', 'C', '1', 'a', 'x-intra-B+C-1']
//  add_variable['B', '1', 'a', 'D', '1', 'a', 'x-intra-B+D-1']
//  add_variable['B', '1', 'a', 'D', '2', 'a', 'x-intra-B+D-1']
//  add_variable['B', '1', 'a', 'D', '3', 'a', 'x-intra-B+D-1']
//  add_variable['B', '1', 'a', 'E', '1', 'a', 'x-intra-B+E-1']
//  add_variable['B', '1', 'a', 'F', '1', 'a', 'x-intra-B+F-1']
//  add_variable['B', '1', 'a', 'G', '1', 'a', 'x-intra-B+G-1']
//  add_variable['B', '1', 'a', 'H', '1', 'a', 'x-intra-B+H-1']
//  add_variable['B', '1', 'a', 'H', '2', 'a', 'x-intra-B+H-1']
//  add_variable['B', '1', 'a', 'H', '3', 'a', 'x-intra-B+H-1']
//  add_variable['C', '1', 'a', 'D', '1', 'a', 'x-intra-C+D-1']
//  add_variable['C', '1', 'a', 'D', '2', 'a', 'x-intra-C+D-1']
//  add_variable['C', '1', 'a', 'D', '3', 'a', 'x-intra-C+D-1']
//  add_variable['C', '1', 'a', 'E', '1', 'a', 'x-intra-C+E-1']
//  add_variable['C', '1', 'a', 'F', '1', 'a', 'x-intra-C+F-1']
//  add_variable['C', '1', 'a', 'G', '1', 'a', 'x-intra-C+G-1']
//  add_variable['C', '1', 'a', 'H', '1', 'a', 'x-intra-C+H-1']
//  add_variable['C', '1', 'a', 'H', '2', 'a', 'x-intra-C+H-1']
//  add_variable['C', '1', 'a', 'H', '3', 'a', 'x-intra-C+H-1']
//  add_variable['D', '1', 'a', 'D', '2', 'a', 'x-intra-D+D-1']
//  add_variable['D', '1', 'a', 'D', '3', 'a', 'x-intra-D+D-1']
//  add_variable['D', '1', 'a', 'E', '1', 'a', 'x-intra-D+E-1']
//  add_variable['D', '1', 'a', 'F', '1', 'a', 'x-intra-D+F-1']
//  add_variable['D', '1', 'a', 'G', '1', 'a', 'x-intra-D+G-1']
//  add_variable['D', '1', 'a', 'H', '1', 'a', 'x-intra-D+H-1']
//  add_variable['D', '1', 'a', 'H', '2', 'a', 'x-intra-D+H-1']
//  add_variable['D', '1', 'a', 'H', '3', 'a', 'x-intra-D+H-1']
//  add_variable['D', '2', 'a', 'D', '3', 'a', 'x-intra-D+D-1']
//  add_variable['D', '2', 'a', 'E', '1', 'a', 'x-intra-D+E-1']
//  add_variable['D', '2', 'a', 'F', '1', 'a', 'x-intra-D+F-1']
//  add_variable['D', '2', 'a', 'G', '1', 'a', 'x-intra-D+G-1']
//  add_variable['D', '2', 'a', 'H', '1', 'a', 'x-intra-D+H-1']
//  add_variable['D', '2', 'a', 'H', '2', 'a', 'x-intra-D+H-1']
//  add_variable['D', '2', 'a', 'H', '3', 'a', 'x-intra-D+H-1']
//  add_variable['D', '3', 'a', 'E', '1', 'a', 'x-intra-D+E-1']
//  add_variable['D', '3', 'a', 'F', '1', 'a', 'x-intra-D+F-1']
//  add_variable['D', '3', 'a', 'G', '1', 'a', 'x-intra-D+G-1']
//  add_variable['D', '3', 'a', 'H', '1', 'a', 'x-intra-D+H-1']
//  add_variable['D', '3', 'a', 'H', '2', 'a', 'x-intra-D+H-1']
//  add_variable['D', '3', 'a', 'H', '3', 'a', 'x-intra-D+H-1']
//  add_variable['E', '1', 'a', 'F', '1', 'a', 'x-intra-E+F-1']
//  add_variable['E', '1', 'a', 'G', '1', 'a', 'x-intra-E+G-1']
//  add_variable['E', '1', 'a', 'H', '1', 'a', 'x-intra-E+H-1']
//  add_variable['E', '1', 'a', 'H', '2', 'a', 'x-intra-E+H-1']
//  add_variable['E', '1', 'a', 'H', '3', 'a', 'x-intra-E+H-1']
//  add_variable['F', '1', 'a', 'G', '1', 'a', 'x-intra-F+G-1']
//  add_variable['F', '1', 'a', 'H', '1', 'a', 'x-intra-F+H-1']
//  add_variable['F', '1', 'a', 'H', '2', 'a', 'x-intra-F+H-1']
//  add_variable['F', '1', 'a', 'H', '3', 'a', 'x-intra-F+H-1']
//  add_variable['G', '1', 'a', 'H', '1', 'a', 'x-intra-G+H-1']
//  add_variable['G', '1', 'a', 'H', '2', 'a', 'x-intra-G+H-1']
//  add_variable['G', '1', 'a', 'H', '3', 'a', 'x-intra-G+H-1']
//  add_variable['H', '1', 'a', 'H', '2', 'a', 'x-intra-H+H-1']
//  add_variable['H', '1', 'a', 'H', '3', 'a', 'x-intra-H+H-1']
//  add_variable['H', '2', 'a', 'H', '3', 'a', 'x-intra-H+H-1']
//  
