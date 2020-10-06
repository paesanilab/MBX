#ifndef MBNRG_3B_A1B4_C1D2_C1D2_DEG3_V1_H
#define MBNRG_3B_A1B4_C1D2_C1D2_DEG3_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1B4_C1D2_C1D2_deg3_v1.h"

/**
 * @file mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.h
 * @brief Contains the structure of the polynomial holder for symmetry A1B4_C1D2_C1D2
 */

/**
 * @namespace mbnrg_A1B4_C1D2_C1D2_deg3
 * @brief Encloses the structure of the polynomial holder for symmetry A1B4_C1D2_C1D2
 */
namespace mbnrg_A1B4_C1D2_C1D2_deg3 {

class mbnrg_A1B4_C1D2_C1D2_deg3_v1 {
   public:
    // Creates an empty class
    mbnrg_A1B4_C1D2_C1D2_deg3_v1(){};

    /**
     * @brief Creates a class and initializes the parameters corresponding to mon1, mon2 and mon3
     * @param[in] mon1 Monomer id of the first monomer of interest
     * @param[in] mon2 Monomer id of the second monomer of interest
     * @param[in] mon3 Monomer id of the third monomer of interest
     */
    mbnrg_A1B4_C1D2_C1D2_deg3_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    // Destroys the class
    ~mbnrg_A1B4_C1D2_C1D2_deg3_v1(){};

    // Polynomial for this symmetry
    typedef poly_A1B4_C1D2_C1D2_deg3_v1 polynomial;

    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] xyz1 Pointer to a double array with the coordinates of the monomers of type mon1.
     * @param[in] xyz2 Pointer to a double array with the coordinates of the monomers of type mon2.
     * @param[in] xyz3 Pointer to a double array with the coordinates of the monomers of type mon3.
     * @param[in] n Number of trimers passed in the xyz arrays.
     * @return Double with the sum of the energies of each trimer.
     */
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);

    /**
     * @brief Computes the two body polynomials for the trimers
     *
     * Given the coordinates of a number of trimers (mon1 in xyz1, mon2 in xyz2, and mon3 in xyz3, it calculates the
     * polynomial value for each one of them, and returns the sum of the contributions for the trimers.
     * @param[in] xyz1 Pointer to a double array with the coordinates of the monomers of type mon1.
     * @param[in] xyz2 Pointer to a double array with the coordinates of the monomers of type mon2.
     * @param[in] xyz3 Pointer to a double array with the coordinates of the monomers of type mon3.
     * @param[in,out] grad1 Pointer to a double array with the gradients of the monomers of type mon1.
     * @param[in,out] grad2 Pointer to a double array with the gradients of the monomers of type mon2.
     * @param[in,out] grad3 Pointer to a double array with the gradients of the monomers of type mon3.
     * @param[in] n Number of trimers passed in the xyz arrays.
     * @param[in,out] virial Vector of 9 elements with the virial tensor.
     * @return Double with the sum of the energies of each trimer.
     */
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    // Values of the non-linear parameters of the polynomials
    double m_k_x_intra_A_B_1;
    double m_k_x_inter_A_C_0;
    double m_k_x_inter_A_D_0;
    double m_k_x_intra_B_B_1;
    double m_k_x_inter_B_C_0;
    double m_k_x_inter_B_D_0;
    double m_k_x_inter_C_C_0;
    double m_k_x_intra_C_D_1;
    double m_k_x_inter_C_D_0;
    double m_k_x_intra_D_D_1;
    double m_k_x_inter_D_D_0;

    // Inner cutoff
    double m_ri = 7.0;

    // Outer cutoff
    double m_ro = 8.0;

    // Switch function
    double f_switch(const double, double &);

    // Values of the linear parameters of the polynomials
    std::vector<double> coefficients;
};

}  // namespace mbnrg_A1B4_C1D2_C1D2_deg3

#endif
