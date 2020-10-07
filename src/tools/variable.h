#ifndef VARIABLE_H
#define VARIABLE_H

#include <cmath>
#include <stdlib.h>

/**
 * @file variable.h
 * @brief Function definitions for different polynomial variable types
 */

/**
 * The Variable structure contains a set of functions to calculate the values of teh different variable types.
 */
struct variable {
    /**
     * @brief Calculates the exp0 variable type
     *
     * Given r0, k, and the coordinates of the two atoms involved, calculates the function f(r) = exp(-k(r-r0)) and its
     * derivative with respect to r.
     * @param[in] r0 Value of r0 in the expression
     * @param[in] k Value of k in the expression
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     * @return Value of f(r)
     */
    double v_exp0(const double& r0, const double& k, const double* p1, const double* p2);

    /**
     * @brief Calculates the exp variable type
     *
     * Given k and the coordinates of the two atoms involved, calculates the function f(r) = exp(-kr) and its derivative
     * with respect to r.
     * @param[in] k Value of k in the expression
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     * @return Value of f(r)
     */
    double v_exp(const double& k, const double* p1, const double* p2);

    /**
     * @brief Calculates the coul0 variable type
     *
     * Given r0, k, and the coordinates of the two atoms involved, calculates the function f(r) = exp(-k(r-r0))/r and
     * its derivative with respect to r.
     * @param[in] r0 Value of r0 in the expression
     * @param[in] k Value of k in the expression
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     * @return Value of f(r)
     */
    double v_coul0(const double& r0, const double& k, const double* p1, const double* p2);

    /**
     * @brief Calculates the coul variable type
     *
     * Given r0, k, and the coordinates of the two atoms involved, calculates the function f(r) = exp(-kr)/r and its
     * derivative with respect to r.
     * @param[in] k Value of k in the expression
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     * @return Value of f(r)
     */
    double v_coul(const double& k, const double* p1, const double* p2);

    /**
     * @brief Calculates the gau0 variable type
     *
     * Given r0, k, and the coordinates of the two atoms involved, calculates the function f(r) = exp[-k(r-r0)^2] and
     * its derivative with respect to r.
     * @param[in] r0 Value of r0 in the expression
     * @param[in] k Value of k in the expression
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     * @return Value of f(r)
     */
    double v_gau0(const double& r0, const double& k, const double* p1, const double* p2);
    /**
         * @brief Calculates the exp0 variable type
         *
         * Given r0, k, and the coordinates of the two atoms involved, calculates the function f(r) = exp(-kr^2) and it
    s derivative with respect to r.
         * @param[in] r0 Value of r0 in the expression
         * @param[in] k Value of k in the expression
         * @param[in] p1 Pointer to the coordinates of first atom
         * @param[in] p2 Pointer to the coordinates of second atom
         * @return Value of f(r)
         */
    double v_gau(const double& k, const double* p1, const double* p2);

    /**
     * @brief Calculates the gradient redistributed to xyz
     *
     * Given the gradient of the variable, applies chain rule to convert it to xyz in both atoms
     * @param[in] gg Value of the gradient you want to resdistribute
     * @param[out] grd1 Pointer to the gradient of the first atom
     * @param[out] grd2 Pointer to the gradient of the second atom
     * @param[in] p1 Pointer to the coordinates of first atom
     * @param[in] p2 Pointer to the coordinates of second atom
     */
    void grads(const double& gg, double* grd1, double* grd2, const double* p1, const double* p2);

    // Temporary storage of the derivative
    double g[3];
};

#endif  // VARIABLE_H
