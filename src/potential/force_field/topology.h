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

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "tools/custom_exceptions.h"
#include "tools/definitions.h"

/**
 * @file topology.h
 * @brief Contains all of the methods common to all of the topology types.
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @brief Base class of bonds, angles, dihedral, inversion. Holds field
 * variables and functions for topology classes
 */
class Topology {
   public:
    /**
     * Default constructor
     */
    Topology();

    /**
     * Default destructor to destroy Toplogy class
     */
    ~Topology();

    /**
     * @brief Obtains the potential energy for the given functional form given
     *        nonlinear and linear parameters
     * @param[in] x The calculated distance, angle, dihedral angle, or
     *            inversion angle
     * @return The potential energy
     */
    virtual double GetEnergy(double x) = 0;

    /**
     * @brief Used to set the linear and nonlinear parameters
     * @param[in] linear_parameters The vector containing the set of
     *            linear parameters
     * @param[in] nonlinear_parameters The vector containing the set of
     *            nonlinear parameters
     */
    virtual void SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters) = 0;

    /**
     * @brief Gets the current nonlinear and linear parameters
     * @param[in,out] linear_parameters Variables to hold the obtained
     *                linear_parameters
     * @param[in,out] nonlinear_parameters Variables to hold the obtained
     *                nonlinear_parameters
     */
    void GetParameters(std::vector<double> &linear_parameters, std::vector<double> &nonlinear_parameters);

    /**
     * @brief Obtain the number of nonlinear parameters
     * @return The number of nonlinear parameters
     */
    void SetNumNonLinear(size_t num_nonlinear);

    /**
     * @brief Obtain the number of linear parameters
     * @return The number of linear parameters
     */
    void SetNumLinear(size_t num_linear);

    /**
     * @brief Obtain the number of nonlinear parameters
     * @return The number of nonlinear parameters
     */
    size_t GetNumNonLinear();

    /**
     * @brief Obtain the number of linear parameters
     * @return The number of linear parameters
     */
    size_t GetNumLinear();

    /**
     * @brief Sets the current indexes for the given topology type
     * @param[in] indexes The vector containing all of the atom indicies
     */
    void SetIndexes(std::vector<size_t> indexes);

    /**
     * @brief Obtain the indicies for the given topology type
     * @return A vector of indicies
     */
    std::vector<size_t> GetIndexes();

    /**
     * @brief Sets the given functional form
     * @param[in] functional_form The functional form that is going to be set
     *            into the field variable
     */
    void SetFunctionalForm(std::string functional_form);

    /**
     * @brief Get the functional form
     * @return The functional form
     */
    std::string GetFunctionalForm();

    /**
     * @brief Sets the topology type
     * @param topology The topology type (bond, angle dihedral, inversion) that
     *        we want to set
     */
    void SetTopology(std::string topology);

    /**
     * @brief Gets the current topology
     * @return The topology (bond, angle, dihedral, inversion)
     */
    std::string GetTopology();

   protected:
    /**
     * The variable to keep track of the connectivity or the topology. E.g. Bond
     */
    std::string topology_;

    /**
     * The functional form
     */
    std::string functional_form_;

    /**
     * The indicies of atoms in the given topology type
     */
    std::vector<size_t> indexes_;

    /**
     * The set of linear parameters associated with the given functional form
     */
    std::vector<double> linear_parameters_;

    /**
     * The set of nonlinear parameters associated with the given functional form
     */
    std::vector<double> nonlinear_parameters_;

    /**
     * The variable to keep track of the number of linear parameters
     */
    size_t num_linear_params_;

    /**
     * The variable to keep track of the number of nonlinear parameters
     */
    size_t num_nonlinear_params_;
};
}  // namespace eff
#endif  // TOPOLOGY_H
