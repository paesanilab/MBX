#ifndef POLY_1B_A1B2_H
#define POLY_1B_A1B2_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly_1b_A1B2_deg4_v1x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for A1B2 symmetry.
 */

/**
 * @namespace x1b_A1B2_deg4
 * @brief Namespace that encloses the A1B2 polynomial structure
 */

namespace x1b_A1B2_deg4 {

struct poly_1b_A1B2_deg4_v1x {
    // Number of variables
    static const unsigned n_vars = 3;
    // Number of terms
    static const unsigned size = 21;

    /**
     * @brief Evaluates the polynomial of degree 3 for A1B2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B2
     * symmetry, and calculates the gradients.
     * @param[in] a Double array of 21 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 3 with the variable values
     * @param[out] g Double array of length 3 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    static double eval(const double a[21], const double x[3], double g[3]);

   public:
    /**
     * @brief Returns the number of variables of the polynomial
     *
     * @return Number of variables of the polynomial
     */
    unsigned report_nvars() { return n_vars; };

    /**
     * @brief Returns the number of terms of the polynomial
     *
     * @return Number of terms of the polynomial
     */
    unsigned report_size() { return size; };
};

}  // namespace x1b_A1B2_deg4

#endif  // POLY_1B_V1X_H
