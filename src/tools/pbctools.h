#ifndef PBCTOOLS_H
#define PBCTOOLS_H

#include <cmath>
#include <vector>
#include <stdlib.h>

/**
 * @file pbctools.h
 * @brief Function definitions for some useful tools for PBC
 */

namespace tools {
/**
 * @brief Relocates point 2 to be minimum image of point 1
 *
 * @param[in] r0 Value of r0 in the expression
 * @param[in] k Value of k in the expression
 * @param[in] p1 Pointer to the coordinates of first atom
 * @param[in] p2 Pointer to the coordinates of second atom
 * @return Value of f(r)
 */
void GetClosePoint(std::vector<double> p1, std::vector<double> &p2, std::vector<double> box,
                   std::vector<double> box_inv);

}  // namespace tools
#endif  // PBCTOOLS_H
