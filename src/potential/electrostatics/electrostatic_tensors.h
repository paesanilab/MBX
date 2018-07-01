/*******************************************************************************
This file contains a class that allows to compute the permanent electric field,
the dipole field, and the gradients for a given pair of sites over all the 
monomers. In the same calculation it modifies fields, electric fields and 
gradients of both sites, returning by reference the values of site i from 
mon 1 and modifying in situ the corresponding ones for site 2. This enables 
vectorization.

!!! IMPORTANT. READ. REALLY, READ. !!!
This functions assume that the data in all the pointers is already CONTIGIOUS
The order assumed is the following:

- XYZ, MU, EFQ, EFD, GRD
FIXME fix this description. This is false. Look at the reorganization in electrostatics.cpp
[BLOCK1][BLOCK2]...[BLOCKN]
where each block is one monomer type organized in the way:
x.1.1 x.1.2 ... x.2.1. x.2.2 ... x.A.B ... y.1.1 y.1.2 ... z.1.1 ... z.A.B
where x.1.2 will be read as coordinate x of site 1 of monomer 2, for B 
monomers with A sites 

- PHI, CHG
[BLOCK1][BLOCK2]...[BLOCKN]
where each block is one monomer type organized in the way:
phi.1.1 phi.1.2 ... phi.2.1 ... phi.A.B 
where phi.1.2 will be read as potential on site 1 of monomer 2, for B 
monomers with A sites

!!! END IMPORTANT !!!

The equations are taken from the manuscript by Masia 
dx.doi.org/10.1063/1.3511713 , J. Chem. Phys. 133, 234101 (2010)

NOTE: The screening functions in this paper are for TTM3, while here we are
using TTM4. The exponents change from 3 to 4 but everything can be trivially 
derived.
*******************************************************************************/
#ifndef ELECTROSTATIC_TENSORS_H
#define ELECTROSTATIC_TENSORS_H

#include <vector>
#include <cmath>
#include <cstddef>

#include "potential/electrostatics/gammq.h" 

namespace elec {

////////////////////////////////////////////////////////////////////////////////

class ElectroTensor {
 public:
  ElectroTensor() {maxnmon = 0;}
  ElectroTensor(size_t n);
  ~ElectroTensor() {}

////////////////////////////////////////////////////////////////////////////////
// Get tensor alpha ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Computes the electric field for a pair of sites for a number of monomers
  // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] > 0

  void CalcT0AndT1WithPolfacNonZero(
    double * xyz1, double * xyz2,     // Coordinates of mon type 1 and 2
    size_t mon1_index,                // Mon 1 index
    size_t mon2_index_start,          // Mon 2 initial index
    size_t mon2_index_end,            // Mon 2 final index
    size_t nmon1, size_t nmon2,       // # monomers of types 1 and 2
    size_t site_i, size_t site_j,     // Site # i of mon1 and # j of mon 2
    double Ai,                        // (polfac[i] * polfac[j]) inverted
    double Asqsq,                     // (polfac[i] * polfac[j])^4
    double aCC, double aCC1_4,        // Thole damping aCC and aCC^(0.25)
    double g34,                       // Gamma ln function. Is a constant.
    // The following pointers correspond to the same array, but they point to
    // the part of the array that contains information of mons 1 and 2
    double * ts0_mon1, // Size n*n    // Electrostatic tensor Tij 
    double * ts0_mon2, // Size n*n    // Electrostatic tensor Tij
    double * ts1_mon1, // Size 3n*n   // Electrostatic tensor Tij_alpha for Mon 1
    double * ts1_mon2);// Size 3n*n   // Electrostatic tensor Tij_alpha for Mon 2

////////////////////////////////////////////////////////////////////////////////

  // Computes the electric field for a pair of sites for a number of monomers
  // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] = 0

  void CalcT0AndT1WithPolfacZero(
    double * xyz1, double * xyz2,
    size_t mon1_index,
    size_t mon2_index_start, size_t mon2_index_end,
    size_t nmon1, size_t nmon2,
    size_t site_i, size_t site_j,
    // The following pointers correspond to the same array, but they point to
    // the part of the array that contains information of mons 1 and 2
    double * ts0_mon1, double * ts0_mon2, 
    double * ts1_mon1, double * ts1_mon2);

////////////////////////////////////////////////////////////////////////////////
// Get Tensor alpha beta ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Computes the dipole field for a pair of sites for a number of monomers
  // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] > 0

  void CalcT1AndT2WithPolfacNonZero(
    double * xyz1, double * xyz2,     // Coordinates of mon type 1 and 2
    size_t mon1_index,                // Mon 1 index
    size_t mon2_index_start,          // Mon 2 initial index
    size_t mon2_index_end,            // Mon 2 final index
    size_t nmon1, size_t nmon2,       // # monomers of types 1 and 2
    size_t site_i, size_t site_j,     // Site # i of mon1 and # j of mon 2
    double Asqsq,                     // (polfac[i] * polfac[j])^4
    double aDD,                       // Thole damping aDD (dipole - dipole)
    size_t nsites,
    // The following pointers correspond to the same array, but they point to
    // the part of the array that contains information of mons 1 and 2
    double * ts1_mon1,  // Size 3n*n  // Electrostatic tensor Tij_alpha for Mon 1
    double * ts1_mon2,  // Size 3n*n  // Electrostatic tensor Tij_alpha for Mon 2
    double * ts2_mon1,  // Size 3n*3n // Electrostatic tensor Tij_alpha_beta
    double * ts2_mon2); // Size 3n*3n // Electrostatic tensor Tij_alpha_beta

////////////////////////////////////////////////////////////////////////////////

  // Computes the dipole field for a pair of sites for a number of monomers
  // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] = 0

  void CalcT1AndT2WithPolfacZero(
    double * xyz1, double * xyz2,
    size_t mon1_index,
    size_t mon2_index_start, size_t mon2_index_end,
    size_t nmon1, size_t nmon2,
    size_t site_i, size_t site_j, 
    size_t nsites,
    double * ts1_mon1, double * ts1_mon2, 
    double * ts2_mon1, double * ts2_mon2);

////////////////////////////////////////////////////////////////////////////////
// GRADIENTS AND ADD DIPOLE CONTRIBUTIONS TO POTENTIAL /////////////////////////
////////////////////////////////////////////////////////////////////////////////

//  // Computes the gradients and updates the potential on sites i and j to 
//  // account for the contribution of the dipoles to the potential phi
//  // for a number of monomers # = mon2_index_end - mon2_index_start when
//  // A=polfac[i] * polfac[j] > 0
//
//  void CalcElecFieldGradsWithPolfacNonZero(
//    double * xyz1, double * xyz2,     // Coordinates of mon type 1 and 2
//    double * chg1, double * chg2,     // Charges of mon type 1 and 2
//    double * mu1, double * mu2,       // Dipoles of mon type 1 and 2
//    size_t mon1_index,                // Mon 1 index
//    size_t mon2_index_start,          // Mon 2 initial index
//    size_t mon2_index_end,            // Mon 2 final index
//    size_t nmon1, size_t nmon2,       // # monomers of types 1 and 2
//    size_t site_i, size_t site_j,     // Site # i of mon1 and # j of mon 2
//    double aDD, double aCD,           // Thole damping aCC and aDD
//    double Asqsq,                     // (polfac[i] * polfac[j])^4
//    double *grdx,                     // Output gradient of site i of mon1 in X
//    double *grdy,                     // Output gradient of site i of mon1 in Y
//    double *grdz,                     // Output gradient of site i of mon1 in Z
//    double *phi1,                     // Output field on site i of mon1
//    double * phi2,                    // Field on site j of mon2
//    double * grd2);                   // Gradient on site j of mon2
//
//////////////////////////////////////////////////////////////////////////////////
//
//  // Computes the gradients and updates the potential on sites i and j to 
//  // account for the contribution of the dipoles to the potential phi
//  // for a number of monomers # = mon2_index_end - mon2_index_start when
//  // A=polfac[i] * polfac[j] = 0
//
//  void CalcElecFieldGradsWithPolfacZero(
//    double * xyz1, double * xyz2,
//    double * chg1, double * chg2,
//    double * mu1, double * mu2,
//    size_t mon1_index,
//    size_t mon2_index_start, size_t mon2_index_end,
//    size_t nmon1, size_t nmon2,
//    size_t site_i, size_t site_j,
//    double *grdx, double *grdy, double *grdz,
//    double *phi1, double * phi2,
//    double * grd2);

////////////////////////////////////////////////////////////////////////////////

 private:

  // Maximum number of monomers that can be in system 
  // For now, is set to the largest number. Later can be set to
  // the maximum number of monomers we will evaluate at once.
  size_t maxnmon; 

  // Vectors that will be reused:
//  std::vector<double> v0_;
//  std::vector<double> v1_;
//  std::vector<double> v2_;
//  std::vector<double> v3_;
//  std::vector<double> v4_;
//  std::vector<double> v5_;
//  std::vector<double> v6_;
//  std::vector<double> v7_;
//  std::vector<double> v8_;
//  std::vector<double> v9_;
//  std::vector<double> v10_;
};

} // namespace elec
#endif
