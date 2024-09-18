
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



#ifndef POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_v2_H
#define POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_v2_H

/**
 * @file poly_3b_A1_B1C2X2_B1C2X2_deg4_v2.h
 * @brief Contains the structure of the polynomial for symmetry A1_B1C2X2_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2
 * @brief Encloses the structure of the polynomial for symmetry A1_B1C2X2_B1C2X2
 */

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2 {

struct poly_A1_B1C2X2_B1C2X2_deg4_v2 {
    // Degree of the polynomial
    static const unsigned degree = 4;

    // Number of variables
    static const unsigned n_vars = 41;

    // Number of terms
    static const unsigned size = 3173;

    /**
     * @brief Evaluates the polynomial of degree 4 for A1_B1C2X2_B1C2X2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1_B1C2X2_B1C2X2 symmetry.
     * @param[in] x Double array of length 41 with the variable values
     * @param[in] a Double array of 3173 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval(const double x[41],
              const double a[3173]);

    /**
     * @brief Evaluates the polynomial of degree 4 for A1_B1C2X2_B1C2X2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1_B1C2X2_B1C2X2 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 41 with the variable values
     * @param[in] a Double array of 3173 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval_direct(const double x[41],
                     const double a[3173]);

    /**
     * @brief Evaluates the polynomial of degree 4 for A1_B1C2X2_B1C2X2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1_B1C2X2_B1C2X2 symmetry.
     * @param[in] x Double array of length 41 with the variable values
     * @param[in] a Double array of 3173 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 41 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval(const double x[41],
              const double a[3173],
                    double g[41]);
    
    /**
     * @brief Evaluates the polynomial of degree 4 for A1_B1C2X2_B1C2X2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, 
     * evaluates the polynomial for the A1_B1C2X2_B1C2X2 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 41 with the variable values
     * @param[in] a Double array of 3173 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 41 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval_direct(const double x[41],
                     const double a[3173],
                           double g[41]);
};

} // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_v2

#endif // POLY_3B_MBNRG_A1_B1C2X2_B1C2X2_DEG4_H



//Polynomial input used to generate these files:

//  add_molecule['A1']
//  add_molecule['B1C2X2']
//  add_molecule['B1C2X2']
//  
//  add_variable['A', '1', 'a', 'B', '1', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '2', 'c', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'C', '1', 'b', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '2', 'b', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '3', 'c', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '4', 'c', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'X', '1', 'b', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '2', 'b', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '3', 'c', 'x-inter-A+X-0']
//  add_variable['A', '1', 'a', 'X', '4', 'c', 'x-inter-A+X-0']
//  add_variable['B', '1', 'b', 'B', '2', 'c', 'x-inter-B+B-0']
//  add_variable['B', '1', 'b', 'C', '1', 'b', 'x-intra-B+C-1']
//  add_variable['B', '1', 'b', 'C', '2', 'b', 'x-intra-B+C-1']
//  add_variable['B', '1', 'b', 'C', '3', 'c', 'x-inter-B+C-0']
//  add_variable['B', '1', 'b', 'C', '4', 'c', 'x-inter-B+C-0']
//  add_variable['B', '1', 'b', 'X', '3', 'c', 'x-inter-B+X-0']
//  add_variable['B', '1', 'b', 'X', '4', 'c', 'x-inter-B+X-0']
//  add_variable['B', '2', 'c', 'C', '1', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'c', 'C', '2', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'c', 'C', '3', 'c', 'x-intra-B+C-1']
//  add_variable['B', '2', 'c', 'C', '4', 'c', 'x-intra-B+C-1']
//  add_variable['B', '2', 'c', 'X', '1', 'b', 'x-inter-B+X-0']
//  add_variable['B', '2', 'c', 'X', '2', 'b', 'x-inter-B+X-0']
//  add_variable['C', '1', 'b', 'C', '2', 'b', 'x-intra-C+C-1']
//  add_variable['C', '1', 'b', 'C', '3', 'c', 'x-inter-C+C-0']
//  add_variable['C', '1', 'b', 'C', '4', 'c', 'x-inter-C+C-0']
//  add_variable['C', '1', 'b', 'X', '3', 'c', 'x-inter-C+X-0']
//  add_variable['C', '1', 'b', 'X', '4', 'c', 'x-inter-C+X-0']
//  add_variable['C', '2', 'b', 'C', '3', 'c', 'x-inter-C+C-0']
//  add_variable['C', '2', 'b', 'C', '4', 'c', 'x-inter-C+C-0']
//  add_variable['C', '2', 'b', 'X', '3', 'c', 'x-inter-C+X-0']
//  add_variable['C', '2', 'b', 'X', '4', 'c', 'x-inter-C+X-0']
//  add_variable['C', '3', 'c', 'C', '4', 'c', 'x-intra-C+C-1']
//  add_variable['C', '3', 'c', 'X', '1', 'b', 'x-inter-C+X-0']
//  add_variable['C', '3', 'c', 'X', '2', 'b', 'x-inter-C+X-0']
//  add_variable['C', '4', 'c', 'X', '1', 'b', 'x-inter-C+X-0']
//  add_variable['C', '4', 'c', 'X', '2', 'b', 'x-inter-C+X-0']
//  add_variable['X', '1', 'b', 'X', '3', 'c', 'x-inter-X+X-0']
//  add_variable['X', '1', 'b', 'X', '4', 'c', 'x-inter-X+X-0']
//  add_variable['X', '2', 'b', 'X', '3', 'c', 'x-inter-X+X-0']
//  add_variable['X', '2', 'b', 'X', '4', 'c', 'x-inter-X+X-0']
//  
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//  
//  add_filter['poorly-behaved']
//  
