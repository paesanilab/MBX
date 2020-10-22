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

#ifndef DIHEDRAL_H
#define DIHEDRAL_H

#include <math.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "topology.h"
#include "tools/custom_exceptions.h"
#include "tools/definitions.h"

/**
 * @file dihedral.h
 * @brief Function definitions for dihedral class
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @brief dihedral class constructs dihedral objects and calculates the
 * potential energy or gradients
 *
 * The dihedral class considers four functional forms: cosine, harmonic,
 * harmonic cosine, and triple cosine
 * Their potentials are as follows:
 * Cosine: A[1 + cos(m phi ijkn - delta)]
 * Harmonic: 0.5k[phi ijkn - phi0]^2
 * Harmonic cosine: 0.5k(cos(phi ijkn) - cos(phi0))^2
 * Triple cosine: 0.5A1(1 + cos(phi ijkn)) + 0.5A2(1 - cos(2 * phi ijkn)) +
 * 0.5A3(1 + cos(3 * phi ijkn))
 *
 * Further information on the potentials can be found at:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
 *
 * Parameter dictionary:
 * Cos:
 * linear_param[0] -> constant A
 * nonlinear_param[0] -> constant m
 * nonlinear_param[0] -> d (delta)
 *
 * Harmonic:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant phi0
 *
 * Harmonic Cosine:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant phi0
 *
 * Triple cosine:
 * linear_param[0] -> constant A1
 * linear_param[1] -> constant A2
 * linear_param[2] -> constant A3
 *
 * The above parameter dictionary also shows how you should enter the parameters
 * in the connectivity file. Below is an example of what each number means
 * dihedral      1         2        3       4      cos         1.0   2.0    3.0
 * ^             ^         ^        ^       ^      ^           ^      ^     ^
 * topology   1st idx  2nd idx   3rd idx  4th idx  func form   A      m     d
 *
 */

class Dihedral : public Topology {
   public:
    /**
     * Default Constructor
     */
    Dihedral();

    /**
     * @brief Overloaded constructor, setting the respective dihedral_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_.
     * @param[in] topology This is the topology, which will be dihedral
     * @param[in] indexes The indexes of the the atoms in the dihedral
     * @param[in] functional_form The functional form that is used to evaluate
     *                            the energy
     */
    Dihedral(std::string topology, std::vector<size_t> indexes, std::string functional_form);

    /**
     * Default Destructor
     */
    ~Dihedral();

    /**
     * @brief Used to set the linear and nonlinear parameters
     * @param[in] linear_parameters The vector containing the set of
     *            linear parameters
     * @param[in] nonlinear_parameters The vector containing the set of
     *            nonlinear parameters
     */
    void SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters);

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the dihedral angle phi ijkn
     * @return The potential energy for a given dihedral angle phi ijkn
     */
    double GetEnergy(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the dihedral angle phi ijkn
     * @return   the gradient
     *
     * Note. this is not the entire gradient. You still need to back propagate
     * this gradient to each of the individual x,y,z components
     */
    double GetTopologyGradient(double x);

    /**
     * @brief Checks if two dihedrals are the same by inspecting
     * all of the field variables.
     * @param[in] dihedral Is the other dihedral object that we are comparing
     * against
     * @return True if the dihedral objects are the same. False otherwise
     */
    bool operator==(Dihedral const &dihedral) const;

   private:
    static constexpr double pi = 3.14159265358979323846;
    static constexpr double two_pi = 2 * pi;
    static constexpr double recip_two_pi = 1 / two_pi;
};
}  // namespace eff
#endif
