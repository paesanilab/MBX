#ifndef DIHEDRAL_H
#define DIHEDRAL_H

#include <math.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "topology.h"
#include "tools/custom_exceptions.h"

/**
 * @file dihedral.h
 * @brief Function definitions for dihedral class
 */

/**
 * @brief dihedral class constructs dihedral objects and calculates the
 * potential energy or non linear value given the linear and non linear
 * parameters
 *
 * The dihedral class considers one functional forms: cosine
 * Their potentials are as follows:
 * Cosine: A[1 + cos(m phi ijkn - delta)]
 *
 * Further information on the potentials can be found at:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
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
     * linear_parameters_ . These are set to 0 to avoid having to call vector
     * push_back in the fittig code.
     * @param[in] dihedral_type This is the dihedral_type of the given dihedral
     * @param[in] indexes The indexes of the the atoms in the dihedral
     * @param[in] functional_form The functional form that is used to fit the
     *            dihedral
     */
    Dihedral(std::string connectivity, size_t dihedral_type, std::vector<size_t> indexes, std::string functional_form);

    /**
     * Default Destructor. Destroys the Dihedral class
     */
    ~Dihedral();

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the dihedral angle phi ijkn
     * @return The potential energy for a given dihedral angle phi ijkn
     */
    double GetEnergy(double x);

    /**
     * @brief Calculate the non linear value given the nonlinear parameters
     * @param[in] x Represents the dihedral angle phi ijkn
     * @return A vector of non linear values. Each one represents the nonlinear
     * term for that functional form.
     */
    std::vector<double> GetNonLinearValue(double x);

    /**
     * @brief Sets the flag of whether a specific dihedral is included or
     * excluded
     * @param[in] includeExclude is a string which is either "include" or
     * "exclude" to specify include or exclude for the dihedral
     */
    void SetIncludeExclude(std::string includeExclude);

    /**
     * @brief Gets the include exclude flag
     * @return a string specifying whether to include or exclude dihedral
     */
    std::string GetIncludeExclude();

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x the dihedral angle
     * @return   the gradient
     *
     * Note. this is not the entire gradient.
     */
    double GetTopologyGradient(double x);

   private:
    /**
     * Used to specify whether to include or exclude the dihedral
     */
    std::string includeExclude_;

    // pi constant
    static constexpr double pi = 3.14159265358979323846;
    static constexpr double two_pi = 2 * pi;
    static constexpr double recip_two_pi = 1 / two_pi;
};

#endif
