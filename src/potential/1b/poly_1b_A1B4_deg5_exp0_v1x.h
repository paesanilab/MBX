#ifndef POLY_1B_A1B4_DEG5_EXP0_H
#define POLY_1B_A1B4_DEG5_EXP0_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly_1b_A1B4_deg5_exp0_v1x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for A1B4 symmetry.
 */

/**
 * @namespace x1b_A1B4_deg5_exp0
 * @brief Namespace that encloses the A1B4 polynomial structure
 */

namespace x1b_A1B4_deg5_exp0 {

struct poly_1b_A1B4_deg5_exp0_v1x {
    // Number of variables
    static const unsigned n_vars = 10;
    // Number of terms
    static const unsigned size = 207;

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B4 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B4
     * symmetry
     * @param[in] a Double array of 207 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 10 with the variable values
     * @return Value of the polynomial
     */
    static double eval(const double a[207], const double x[10]);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B4 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B4
     * symmetry, and calculates the gradients.
     * @param[in] a Double array of 207 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 10 with the variable values
     * @param[out] g Double array of length 10 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    static double eval(const double a[207], const double x[10], double g[10]);

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

}  // namespace x1b_A1B4_deg5_exp0

#endif  // POLY_1B_A1B4_DEG5_EXP0_H
