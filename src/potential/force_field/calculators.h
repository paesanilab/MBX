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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include "tools/custom_exceptions.h"
#include "tools/definitions.h"

/**
 * @file calculators.h
 * @brief Function definitions for all of the calculators needed for fitting
 * code.
 *
 * There are a total of 4 main calculators: distanceCalculator,
 * angleCalculator, dihedralAngleCalculator, and inversionAngleCalculator. All
 * other functions are helper functions. All formulas used to calculate each
 * of the distances or angles is given in the dlpoly manual.
 *
 * See the following for more detailed explanations of the potentials and their
 * functional forms:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @brief CalculateDistance Calculates the euclidean distance between two
 *        sets of coordinates
 *
 * The input vectors look like: <x,y,z>. The units of the return value
 * matches the input units
 * @param[in]  coor1 Is the xyz coordinates of the first atom
 * @param[in]  coor2 Is the xyz coordinates of the second atom
 * @return The euclidean distance between the two points
 */
double CalculateDistance(std::vector<double> coor1, std::vector<double> coor2);

/**
 * @brief CalculateDistance Calculates the euclidean distance for a given
 * directional vector
 *
 * @param[in] directional_vec Is the directional vector between two coordinates
 * @return The euclidean distance between the two points
 */
double CalculateDistance(std::vector<double> directional_vec);

/**
 * @brief calculateAngle Calculates the geometric angle between three
 *        coordinates
 *
 * The angle is calculated using the law of cosines. Further information about
 * the formula can be found online. coor2 MUST be the central atom.
 * @param[in]  coor1 Is the xyz coordinates of the first atom
 * @param[in]  coor2 Is the xyz coordinates of the central atom
 * @param[in]  coor3 Is the xyz coordinates of the third atom
 * @return The geometric angle between the three coordinates in radians
 */
double CalculateAngle(std::vector<double> coor1, std::vector<double> coor2, std::vector<double> coor3);

/**
 * @brief calculateDihedralAngle Calculates the dihedral angle between
 *        four atoms.
 *
 * If our dihedral is A-B-C-D. Then our first outer atom can be either A or D,
 * The order is unimportant in this case. The order that is important is that
 * once you have selected the first outer atom, you must work your way to
 * the other other outer atom without jumping over other atoms. In other words,
 * do NOT flip the order of the inner two atoms.
 * Example1: If coor1 is A, then coor2 MUST be B, coor3 MUST be C, and coor4
 * MUST be D.
 * Example2: If coor1 is D, then coor2 MUST be C, coor3 MUST be B, and coor4
 * MUST be A.
 * You can not do: coor1 is A
 *                 coor2 is C
 *                 coor3 is B
 *                 coor4 is D.
 * @param[in]  coor1 Is the xyz coordinate of the first outer atom.
 * @param[in]  coor2 Is the xyz coordinate of the first inner atom.
 * @param[in]  coor3 Is the xyz coordinate of the second inner atom.
 * @param[in]  coor4 Is the xyz coordinate of the second outer atom.
 * @return The dihedral angle, phi, in radians of the four coordinate system
 */
double CalculateDihedralAngle(std::vector<double> coor1, std::vector<double> coor2, std::vector<double> coor3,
                              std::vector<double> coor4);

/**
 * @brief CalculateGradB is used to compute the gradient of B for each index and
 * component and then uses it to update the vector of gradients appropriately
 *
 * @param[in]  coor1 Is the xyz coordinate of the first outer atom.
 * @param[in]  coor2 Is the xyz coordinate of the first inner atom.
 * @param[in]  coor3 Is the xyz coordinate of the second inner atom.
 * @param[in]  coor4 Is the xyz coordinate of the second outer atom.
 * @param[in]  indexes is the vector of atom indexes in the dihedral. STARTS at 1!
 * @param[in]  cummu_grad is the constant used to compute full gradient. See
 * eval.cpp for the function
 * @param[in]  gradients is a pointer to the gradients vector. Used by function
 * to update gradients
 * @return Nothing
 */
void CalculateGradB(std::vector<double> coor1, std::vector<double> coor2, std::vector<double> coor3,
                    std::vector<double> coor4, std::vector<size_t> indexes, double cummu_grad, double phi,
                    std::vector<double>& gradients, int mon_num, int nat, std::vector<double>& curr_force);

/**
 * @brief calculateCrossProduct Calculates the cross product between two sets
 *        of vectors
 *
 * calculateCrossProduct does NOT take coordinates like the previous three
 * methods. Instead it takes vectors as input. For more information on how to
 * calculate cross products, just google it for the formula
 * @param[in]  v1 This is the first vector to be crossed
 * @param[in]  v2 This is the second vector to be crossed
 * @return Returns the crossed normal vector
 */
std::vector<double> CalculateCrossProduct(std::vector<double> v1, std::vector<double> v2);

/**
 * @brief calculateDotProduct Calculates the dot product between two sets of
 *        vectors
 * @param[in]  v1 This is the first vector to be dotted
 * @param[in]  v2 This is the second vector to be dotted
 * @return Returns the dot product of the two vectors
 */
double CalculateDotProduct(std::vector<double> v1, std::vector<double> v2);

/**
 * @brief calculateInversionAngle Calculates the inversion angle between four
 *        sets of given coordinates
 *
 * calculateInversionAngle's first argument MUST be the central atom of the
 * inversion. The remaining three coordinates can be in any order. This method
 * works by considering all possible permutations of ijkn angle combinations
 * to calculate three sets of inversion angles. Further information can be found
 * in the dlpoly manual on the specific of the formula
 *
 * @param[in]  coor1 This is the coordinate of the central atom
 * @param[in]  coor2 This is the second coordinate in the system.
 * @param[in]  coor3 This is the third coordinate in the system.
 * @param[in]  coor4 This is the fourth coordinate in the system.
 * @return A vector containing all of the inversion angles in radians.
 */
std::vector<double> CalculateInversionAngle(std::vector<double> coor1, std::vector<double> coor2,
                                            std::vector<double> coor3, std::vector<double> coor4);

/**
 * @brief CalculateInversionGrad is used to compute the remaining gradient of
 * the inversion potential. It then uses it to update the vector of gradients appropriately
 * Note that this function only calculates the tedious derivative of:
 * [(rij dot ukn)^2 + (rij dot vkn)^2]^1/2 / rij    See dlpoly 2.65 and 2.66 to
 * understand what this is
 *
 * @param[in]  centralCoor Is the xyz coordinate of the central atom.
 * @param[in]  coor2 Is the xyz coordinate of the connected atom.
 * @param[in]  coor3 Is the xyz coordinate of the connected atom.
 * @param[in]  coor4 Is the xyz coordinate of the connected atom.
 * @param[in]  indexes is the vector of atom indexes in the inversion.
 * @param[in]  cummu_grad is the vectors holding the functional form derivative.
 * In dlpoly these are the gamb, gamc, gamd
 * @param[in]  phis is a vector holding all of the inversion angles
 * @param[in]  gradients is a reference to the gradients vector. Used by function
 * to update gradients
 * @return Nothing
 */
void CalculateInversionGrad(std::vector<double> centralCoor, std::vector<double> coor2, std::vector<double> coor3,
                            std::vector<double> coor4, std::vector<size_t> indexes, std::vector<double> cummu_grad,
                            std::vector<double> phis, std::vector<double>& gradients, int mon_num, int nat,
                            std::vector<double>& curr_force);
/**
 * @brief Helper function for inversion gradient calculator. Fills the vectors
 * ukn, vkn, unj, vnj, ujk, vjk with the appropriate values. This is OPERATION
 * SENSITIVE due to subtraction. This means: what you pass as rix_norm and riy_norm needs to follow
 * exactly what dlpoly stipulates.
 *
 * @param[in]  rix_norm. x indicates either j,k,n. Passed from CalculateInversionGrad
 * @param[in]  riy_norm. y indicates either j,k,n. y should not be the same as x
 * above. Passed from CalculateInversionGrad
 * @param[in]  u_vec is the u vector from CalculateInversionGrad
 * @param[in]  v_vec is the v vector from CalculateInversionGrad
 *
 * @return nothing
 */
void GetVecUVecV(std::vector<double> rix_norm, std::vector<double> riy_norm, std::vector<double>& u_vec,
                 std::vector<double>& v_vec);

/**
 * @brief Helper function for inversion gradient calculator. Fills the vector
 * wkn with the appropriate values
 *
 * @param[in]  riz. where z is either j,k,n. z is unique from x and y.
 * Passed from CalculateInversionGrad
 * @param[in]  uxy_norm is the unit vector and x and y are either j,k,n. x != y.
 * Passed from CalculateInversionGrad
 * @param[in]  uxy_norm is the unit vector and x and y are either j,k,n. x != y.
 * Passed from CalculateInversionGrad
 * @return wxy and x and y are either j,k,n. x != y. Passed pack to CalculateInversionGrad
 */
std::vector<double> GetVecW(std::vector<double>& riz, std::vector<double>& uxy_norm, std::vector<double>& vxy_norm);

/**
 * @brief Helper function to compute the unit vector of a vector
 *
 * @param[in]  vec is the vector to be normalized
 *
 * @return the normalized vector
 */
std::vector<double> NormalizeVector(std::vector<double> vec);

/**
 * @brief Helper function to determine if a value is nonzero
 *
 * @param[in] val is the value that we want to check
 *
 * @return val if non zero, otherwise 1.0e-11
 */
double CheckNonZero(double val);
}  // namespace eff
#endif
