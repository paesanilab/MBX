#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
/**
 * @file topology.h
 * @brief Contains all of the methods common to all of the topology types.
 */

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
     * @brief Gets the current nonlinear and linear parameters
     * @param[in,out] linear_parameters Variables to hold the obtained
     *                linear_parameters
     * @param[in,out] nonlinear_parameters Variables to hold the obtained
     *                nonlinear_parameters
     */
    void GetParameters(std::vector<double> &linear_parameters, std::vector<double> &nonlinear_parameters);

    /**
     * @brief Obtain the nonlinearvalue given the nonlinear parameters
     * @param[in] x The calculated distance, angle, dihedral angle, or
     *            inversion angle
     * @return A vector containing all of the nonlinear values
     */
    virtual std::vector<double> GetNonLinearValue(double x) = 0;

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
     * @brief Used to set the linear and nonlinear parameters
     * @param[in] linear_parameters The vector containing the set of
     *            linear parameters
     * @param[in] nonlinear_parameters The vector containing the set of
     *            nonlinear parameters
     */
    void SetParameters(std::vector<double> linear_parameters, std::vector<double> nonlinear_parameters);

    /**
     * @brief Used to set the nonlinear parameters. Only used to fit linear as nonlinear
     * @param parameters [description]
     */
    void SetParameters(std::vector<double> parameters);

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
     * @brief Set the current topology type
     * @param[in] topology_type The given topology type
     */
    void SetTopologyType(size_t topology_type);

    /**
     * @brief Get the current topology type
     * @return A value indicating the topology type
     */
    size_t GetTopologyType();

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
     * @brief Allows derived classes to set the linear flag
     */
    void SetLinearFlag(bool linear);

    /**
     * @brief Gets the current state of the linear_ flag
     * @return The linear_ flag
     */
    bool GetLinearFlag();

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

    /**
     * @brief Prints the linear and nonlinear parameters in a formatted manner to the
     *        console
     */
    void PrintParameters();

   protected:
    /**
     * The bond type
     */
    // size_t bond_type_;

    /**
     * The angle type
     */
    // size_t angle_type_;

    /**
     * The dihedral type
     */
    // size_t dihedral_type_;

    /**
     * The inversion type
     */
    // size_t inversion_type_;

    /**
     * The variable to keep track of the connectivity or the topology. E.g. Bond
     */
    std::string topology_;

    /**
     * The topology type. e.g. bond type 1
     */
    size_t topology_type_;
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
     * The boolean variable to indicate to fit the linear parameters in a non-
     * linear way. If linear_ is true, then we want to fit linear terms in a
     * linear way.
     */
    bool linear_;

    /**
     * The variable to keep track of the number of linear parameters
     */
    size_t num_linear_params_;

    /**
     * The variable to keep track of the number of nonlinear parameters
     */
    size_t num_nonlinear_params_;
};

#endif  // TOPOLOGY_H
