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

#ifndef BOND_H
#define BOND_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "topology.h"
#include "tools/custom_exceptions.h"
#include "tools/definitions.h"

/**
 * @file bond.h
 * @brief Function definitions for bond class
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @brief Bond class constructs bond objects and calculates the
 * potential energy and gradients
 *
 * The bond class considers three functional forms: harmonic, morse, and quartic
 * Their potentials are as follows:
 * Harmonic: 0.5 * k * ( rij - r0 ) * ( rij - r0 )
 * Morse: E0[{1-e^(-k(rij - r0))}^2]
 * Quartic: k' / 2 * ( rij - r0 ) * ( rij - r0 ) +
 *          k'' / 3 * ( rij - r0 ) * ( rij - r0 ) * ( rij - r0 )
 *          k''' / 4 * ( rij - r0 ) * ( rij - r0 ) * ( rij - r0 ) * ( rij - r0 )
 *
 * Further information on the potentials can be found at:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
 *
 *
 * Parameter dictionary:
 * Harmonic:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant r0
 *
 * Morse:
 * linear_param[0] -> constant E0
 * nonlinear_param[0] -> constant k
 * nonlinear_param[1] -> constant r0
 *
 * Quartic:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant r0
 * linear_param[1] -> constant k'
 * linear_param[2] -> constant k''
 *
 * The above parameter dictionary also shows how you should enter the parameters
 * in the connectivity file. Below is an example of what each number means
 * bond         1         2             harm            1.0    2.0
 * ^            ^         ^               ^             ^      ^
 * topology    1st idx  2nd idx      func form          k      r0
 *
 */

class Bond : public Topology {
   public:
    /**
     * Default constructor
     */
    Bond();

    /**
     * @brief Overloaded constructor, setting the respective topology, bond_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_ .
     * @param[in] topology This is the topology, which will be bond
     * @param[in] indexes The indexes of the the atoms in the bond
     * @param[in] functional_form The functional form that is used to evaluate
     *                            the energy
     */
    Bond(std::string topology, std::vector<size_t> indexes, std::string functional_form);

    /**
     * Default deconstructor
     */
    ~Bond();

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
     * @param[in] x Represents the distance rij,
     * @return The potential energy for a given bond length rij
     */
    double GetEnergy(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the bond distance rij
     * @return   the gradient
     *
     * Note. this is not the entire gradient. You still need to back propagate
     * this gradient to each of the individual x,y,z components
     */
    double GetTopologyGradient(double x);

    /**
     * @brief Checks if two bonds are the same by inspecting
     * all of the field variables.
     * @param[in] bond Is the other bond object that we are comparing
     * against
     * @return True if the bond objects are the same. False otherwise
     */
    bool operator==(Bond const &bond) const;

   private:
};
}  // namespace eff
#endif  // BOND_H
