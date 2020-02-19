#ifndef INVERSION_H
#define INVERSION_H

#include <math.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "topology.h"
#include "tools/custom_exceptions.h"

/**
 * @file inversion.h
 * @brief Function definitions for inversion class
 */

/**
 * @brief Inversion class constructs inversion objects and calculates the
 * potential energy or non linear value given the linear and non linear
 * parameters
 *
 * The inversion class considers one functional form: harmonic
 * Their potentials are as follows:
 * Harmonic: 0.5 * k * ( phi ijkn - phi 0 ) * ( phi ijkn - phi 0 )
 *
 * Important note: There is a discretion between this class and how dlpoly
 * handles inversions. In dlpoly, you are expected to input the permuted
 * indexes of the inversion as SEPARATE inversions. E.g: dlpoly expects:
 * inversions 3
 * harm i  j  k  n  linear_param   non_linear_param
 * harm i  n  j  k  linear_param   non_linear_param
 * harm i  k  n  j  linear_param   non_linear_param
 *
 * Whereas in this code, it will do the permutation for you without you putting
 * it in the connectivity file. E.g.: This code only expects one line:
 * inversion i  j  k  n  connectivity_type  func_form  fit_linear_or_nonlinear
 *
 * Further information on the potentials can be found at:
 * ftp://ftp.dl.ac.uk/ccp5/DL_POLY/DL_POLY_CLASSIC/DOCUMENTS/USRMAN2.19.pdf
 */

class Inversion : public Topology {
   public:
    /**
     * Default constructor. Creates an inversion class
     */
    Inversion();

    /**
     * @brief Overloaded constructor, setting the respective inversion_type
     * indexes, and functional_form.
     *
     * This constructor sets the field variables. It also puts the respective
     * number of 0's onto the vectors nonlinear_parameters_ and
     * linear_parameters_ . These are set to 0 to avoid having to call vector
     * push_back in the fittig code.
     * @param[in] inversion_type This is the inversion_type of the given inversion
     * @param[in] indexes The indexes of the the atoms in the inversion angle
     * @param[in] functional_form The functional form that is used to fit the
     *            inversion angle
     */
    Inversion(std::string connectivity, size_t inversion_type, std::vector<size_t> indexes,
              std::string functional_form);

    /**
     * Default Destructor. Destroys the inversion class
     */
    ~Inversion();

    /**
     * @brief Calculates the potential energy using the nonlinear and linear
     *        parameters
     * @param[in] x Represents the inversion angle phi ijkn,
     * @return The potential energy for a given inversion angle phi ijkn
     */
    double GetEnergy(double x);

    /**
     * @brief Calculate the non linear value given the nonlinear parameters
     * @param[in] x Represents the inversion angle phi ijkn
     * @return A vector of non linear values. Each one represents the nonlinear
     * term for that functional form.
     */
    std::vector<double> GetNonLinearValue(double x);

    /**
     * @brief gets the gradient for this particular topology for a given
     *        functional form
     * @param  x inversion angle
     * @return   the gradient
     *
     * Note. this is not the entire gradient.
     */
    double GetTopologyGradient(double x);

    /**
     * @brief Checks if two inversions are the same by inspecting
     * all of the field variables.
     * @param[in] inversion Is the other inversion object that we are comparing
     * against
     * @return True if the inversion objects are the same. False otherwise
     */
    bool operator==(Inversion const &inversion) const;

   private:
};

#endif
