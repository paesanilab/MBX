#ifndef ANGLES_H
#define ANGLES_H

#include <iostream>
#include <string>
#include <vector>

#include "topology.h"
#include "tools/custom_exceptions.h"

/**
 * @file angles.h
 * @brief Function definitions for angles class
 */

/**
 * @brief Angles class constructs angles objects and calculates the
 * potential energy or non linear value given the linear and non linear
 * parameters
 *
 * The angle class considers two functional forms: harmonic and quartic
 * Their potentials are as follows:
 * Harmonic: 0.5 * k * ( theta ij - theta 0 ) * ( theta ij - theta 0 )
 * Quartic: k' / 2 * ( theta ij - theta 0 ) * ( theta ij - theta 0 ) +
 *          k'' / 3 * ( theta ij - theta 0 ) * ( theta ij - theta 0 ) * ( theta ij - theta 0 )
 *          k''' / 4 * ( theta ij - theta 0 ) * ( theta ij - theta 0 ) * ( theta ij - theta 0 ) * ( theta ij - theta 0 )
 *
 * Further information on the potentials can be found at:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
 */
class Angles : public Topology {
   public:
    /**
     * Default constructor. Creates a angle class
     */
    Angles();

    /**
     * @brief Overloaded constructor, setting the respective angle_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_ . These are set to 0 to avoid having to call vector
     * push_back in the fittig code.
     * @param[in] angle_type This is the angle type of the given angle
     * @param[in] indexes The indexes of the the atoms in the angle
     * @param[in] functional_form The functional form that is used to fit the
     *            angle
     */
    Angles(std::string connectivity, size_t angle_type, std::vector<size_t> indexes, std::string functional_form);

    /**
     * Default Destructor. Destroys the Angles class
     */
    ~Angles();

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the angle theta ij,
     * @return The potential energy for a given angle theta ij
     */
    double GetEnergy(double x);

    /**
     * @brief Calculate the non linear value given the nonlinear parameters
     * @param[in] x Represents the distance theta ij
     * @return A vector of non linear values. Each one represents the nonlinear
     * term for that functional form.
     */
    std::vector<double> GetNonLinearValue(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the angle
     * @return   the gradient
     *
     * Note. this is not the entire gradient.
     */
    double GetTopologyGradient(double x);

   private:
};

#endif
