#ifndef BOND_H
#define BOND_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "topology.h"
#include "tools/custom_exceptions.h"

/**
 * @file bond.h
 * @brief Function definitions for bond class
 */

/**
 * @brief Bond class constructs bond objects and calculates the
 * potential energy or non linear value given the linear and non linear
 * parameters
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
 */

class Bond : public Topology {
   public:
    /**
     * Default constructor. Creates a Bond class
     */
    Bond();

    /**
     * @brief Overloaded constructor, setting the respective bond_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_ . These are set to 0 to avoid having to call vector
     * push_back in the fittig code.
     * @param[in] connectivity This is the topology, which will be Bond
     * @param[in] bond_type This is the bond_type of the given bond
     * @param[in] indexes The indexes of the the atoms in the bond
     * @param[in] functional_form The functional form that is used to fit the
     *            bond
     */
    Bond(std::string connectivity, size_t bond_type, std::vector<size_t> indexes, std::string functional_form);

    ~Bond();

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the distance rij,
     * @return The potential energy for a given bond length rij
     */
    double GetEnergy(double x);

    /**
     * @brief Calculate the non linear value given the nonlinear parameters
     * @param[in] x Represents the distance rij
     * @return A vector of non linear values. Each one represents the nonlinear
     * term for that functional form.
     */
    std::vector<double> GetNonLinearValue(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the bond length
     * @return   the gradient
     *
     * Note. this is not the entire gradient. You still need to manipulate this
     * with the x,y,z.
     */
    double GetTopologyGradient(double x);

   private:
};

#endif  // BOND_H
