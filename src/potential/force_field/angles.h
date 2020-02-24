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
 * potential energy or gradients
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
 *
 *
 * Parameter dictionary:
 * Harmonic:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant theta0
 *
 * Quartic:
 * linear_param[0] -> constant k
 * nonlinear_param[0] -> constant theta0
 * linear_param[1] -> constant k'
 * linear_param[2] -> constant k''
 *
 * The above parameter dictionary also shows how you should enter the parameters
 * in the connectivity file. Ie:
 * Angle 1 2 3 1 quartic 1.0 2.0 3.0 4.0
 * will set:
 * k -> 1.0
 * theta0 -> 2.0
 * k' -> 3.0
 * k'' -> 4.0
 *
 */

class Angles : public Topology {
   public:
    /**
     * Default constructor
     */
    Angles();

    /**
     * @brief Overloaded constructor, setting the respective topology, angle_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_.
     * @param[in] topology This is the topology, which will be angle
     * @param[in] angle_type This is the angle type of the given angle
     * @param[in] indexes The indexes of the the atoms in the angle
     * @param[in] functional_form The functional form that is used to evaluate
     *                            the energy
     */
    Angles(std::string topology, size_t angle_type, std::vector<size_t> indexes, std::string functional_form);

    /**
     * Default Destructor.
     */
    ~Angles();

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the theta,
     * @return The potential energy for a given theta
     */
    double GetEnergy(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the angle theta
     * @return   the gradient
     *
     * Note. this is not the entire gradient. You still need to back propagate
     * this gradient to each of the individual x,y,z components
     */
    double GetTopologyGradient(double x);

    /**
     * @brief Checks if two angles are the same by inspecting
     * all of the field variables.
     * @param[in] angle Is the other angle object that we are comparing
     * against
     * @return True if the angle objects are the same. False otherwise
     */
    bool operator==(Angles const &angle) const;

   private:
};

#endif
