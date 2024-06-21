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
#ifndef FIELDS_H
#define FIELDS_H

#include <vector>
#include <cstddef>


#ifdef __INTEL_COMPILER
#include <mathimf.h>
#endif

#include <functional>
#include <unordered_map>

#include "potential/electrostatics/gammq.h"
#include "tools/constants.h"
#include "tools/definitions.h"


namespace elec {
    struct PrecomputedInfo {
        std::vector<size_t> good_mon2;
        std::vector<double> reordered_xyz2;
        std::vector<size_t> reordered_islocal;
        std::vector<double> reordered_mu2;
        std::vector<double> reordered_Efd2;
        std::vector<double> rijx;
        std::vector<double> rijy;
        std::vector<double> rijz;
        std::vector<double> ts2x;
        std::vector<double> ts2y;
        std::vector<double> ts2z;
        std::vector<double> s1r3;
    };

    typedef std::tuple<size_t, size_t, size_t, size_t, size_t> key_precomputed_info;
    struct key_hash {
        std::size_t operator()(const key_precomputed_info& k) const {
            return std::get<0>(k) << 8 ^ std::get<1>(k) << 6 ^ std::get<2>(k) ^ std::get<3>(k) << 4 ^ std::get<4>(k) << 2;
            // return std::hash<key_precomputed_info>{}(k);
        }
    };
////////////////////////////////////////////////////////////////////////////////

class ElectricFieldHolder {
   public:
    ElectricFieldHolder() { maxnmon = 0; }
    ElectricFieldHolder(size_t n);
    ~ElectricFieldHolder() {}

    ////////////////////////////////////////////////////////////////////////////////
    // PERMANENT ELECTRIC FIELD ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Computes the electric field for a pair of sites for a number of monomers
    // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] > 0

    void CalcPermanentElecField(double *xyz1, double *xyz2,    // Coordinates of mon type 1 and 2
                                double *chg1, double *chg2,    // Charges of mon type 1 and 2
                                size_t mon1_index,             // Mon 1 index
                                size_t mon2_index_start,       // Mon 2 initial index
                                size_t mon2_index_end,         // Mon 2 final index
                                size_t nmon1, size_t nmon2,    // # monomers of types 1 and 2
                                size_t site_i, size_t site_j,  // Site # i of mon1 and # j of mon 2
                                double Ai,                     // (polfac[i] * polfac[j]) inverted
                                double Asqsqi,                 // (polfac[i] * polfac[j])^4 inverted
                                double aCC, double aCC1_4,     // Thole damping aCC and aCC^(0.25)
                                double g34,                    // Gamma ln function. Is a constant.
                                double *Efqx_mon1,             // Output electric field on X for Mon 1
                                double *Efqy_mon1,             // Output electric field on Y for Mon 1
                                double *Efqz_mon1,             // Output electric field on Z for Mon 1
                                double *phi1,                  // Output potential for mon 1
                                double *phi2,                  // Potential on Mon 2
                                double *Efq2,                  // Electric field on Mon 2
                                double elec_scale_factor,      // Scale factor for electrostatic (1=included, 0=excluded)
                                double ewald_alpha,            // Ewald attenuation paramter
                                bool use_pbc,                  // Whether to enforce periodic boundary conditions
                                const std::vector<double> &box,          // The lattice vectors
                                const std::vector<double> &box_inverse,  // The inverse lattice vectors
                                double cutoff,                           // The real space cutoff for pairs
                                bool use_ghost,                          // use ghost monomers
                                const std::vector<size_t> &islocal, const size_t isl1_offset, const size_t isl2_offset,
                                const size_t m2_offset,
                                std::vector<double> *virial = 0);  // The virial);

    // Computes the electric field for a pair of sites for a number of monomers
    // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] > 0
    // THIS VERSION OF THE FUNCTION USES PRECOMPUTED INFORMATION TO ENABLE BETTER SIMD AND FEWER ARITHMETIC OPERATIONS

    void CalcPermanentElecField_Optimized(double *xyz1, double *xyz2,    // Coordinates of mon type 1 and 2
                                double *chg1, double *chg2,    // Charges of mon type 1 and 2
                                size_t mon1_index,             // Mon 1 index
                                size_t mon2_index_start,       // Mon 2 initial index
                                size_t mon2_index_end,         // Mon 2 final index
                                size_t nmon1, size_t nmon2,    // # monomers of types 1 and 2
                                size_t site_i, size_t site_j,  // Site # i of mon1 and # j of mon 2
                                double Ai,                     // (polfac[i] * polfac[j]) inverted
                                double Asqsqi,                 // (polfac[i] * polfac[j])^4 inverted
                                double aCC, double aCC1_4,     // Thole damping aCC and aCC^(0.25)
                                double g34,                    // Gamma ln function. Is a constant.
                                double *Efqx_mon1,             // Output electric field on X for Mon 1
                                double *Efqy_mon1,             // Output electric field on Y for Mon 1
                                double *Efqz_mon1,             // Output electric field on Z for Mon 1
                                double *phi1,                  // Output potential for mon 1
                                double *phi2,                  // Potential on Mon 2
                                double *Efq2,                  // Electric field on Mon 2
                                double elec_scale_factor,      // Scale factor for electrostatic (1=included, 0=excluded)
                                double ewald_alpha,            // Ewald attenuation paramter
                                bool use_pbc,                  // Whether to enforce periodic boundary conditions
                                const std::vector<double> &box,          // The lattice vectors
                                const std::vector<double> &box_inverse,  // The inverse lattice vectors
                                double cutoff,                           // The real space cutoff for pairs
                                bool use_ghost,                          // use ghost monomers
                                const std::vector<size_t> &islocal, const size_t isl1_offset, const size_t isl2_offset,
                                const size_t m2_offset,
                                PrecomputedInfo& precomputedInformation, // Contains precomputed coordinate-dependant calculations
                                std::vector<double> *virial = 0);  // The virial);

    ////////////////////////////////////////////////////////////////////////////////
    // DIPOLE ELECTRIC FIELD ///////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Computes the dipole field for a pair of sites for a number of monomers
    // # = mon2_index_end - mon2_index_start when A=polfac[i] * polfac[j] > 0

    void CalcDipoleElecField(double *xyz1, double *xyz2,              // Coordinates of mon type 1 and 2
                             double *mu1, double *mu2,                // Dipoles of mon type 1 and 2
                             size_t mon1_index,                       // Mon 1 index
                             size_t mon2_index_start,                 // Mon 2 initial index
                             size_t mon2_index_end,                   // Mon 2 final index
                             size_t nmon1, size_t nmon2,              // # monomers of types 1 and 2
                             size_t site_i, size_t site_j,            // Site # i of mon1 and # j of mon 2
                             double Asqsqi,                           // (polfac[i] * polfac[j])^4 inverted
                             double aDD,                              // Thole damping aDD (dipole - dipole)
                             double *Efd2,                            // Electric field on Mon 2
                             double *Efdx_mon1,                       // Output electric field on X for Mon 1
                             double *Efdy_mon1,                       // Output electric field on Y for Mon 1
                             double *Efdz_mon1,                       // Output electric field on Z for Mon 1
                             double ewald_alpha,                      // Ewald attenuation paramter
                             bool use_pbc,                            // Whether to enforce periodic boundary conditions
                             const std::vector<double> &box,          // The lattice vectors
                             const std::vector<double> &box_inverse,  // The inverse lattice vectors
                             double cutoff,                           // The real space cutoff for pairs
                             bool use_ghost,                          // use ghost monomers
                             const std::vector<size_t> &islocal, 
                             const size_t isl1_offset, 
                             const size_t isl2_offset);
    
    // Optimized version of CalcDipoleElecField
    void CalcDipoleElecField_WithinCutoff(double *xyz1, double *xyz2,              // Coordinates of mon type 1 and 2
                                          double *mu1, double *mu2,                // Dipoles of mon type 1 and 2
                                          size_t mon1_index,                       // Mon 1 index
                                          size_t mon2_index_start,                 // Mon 2 initial index
                                          size_t mon2_index_end,                   // Mon 2 final index
                                          size_t nmon1, size_t nmon2,              // # monomers of types 1 and 2
                                          size_t site_i, size_t site_j,            // Site # i of mon1 and # j of mon 2
                                          double Asqsqi,                           // (polfac[i] * polfac[j])^4 inverted
                                          double aDD,                              // Thole damping aDD (dipole - dipole)
                                          double *Efd2,                            // Electric field on Mon 2
                                          double *Efdx_mon1,                       // Output electric field on X for Mon 1
                                          double *Efdy_mon1,                       // Output electric field on Y for Mon 1
                                          double *Efdz_mon1,                       // Output electric field on Z for Mon 1
                                          double ewald_alpha,                      // Ewald attenuation paramter
                                          bool use_pbc,                            // Whether to enforce periodic boundary conditions
                                          const std::vector<double> &box,          // The lattice vectors
                                          const std::vector<double> &box_inverse,  // The inverse lattice vectors
                                          double cutoff,                           // The real space cutoff for pairs
                                          bool use_ghost,                          // use ghost monomers
                                          const std::vector<size_t> &islocal, 
                                          const size_t isl1_offset,
                                          const size_t isl2_offset);
    
    // Optimized version of CalcDipoleElecField that builds off of CalcDipoleElecField_WithinCutoff
    void CalcDipoleElecField_Optimized(double *xyz1, double *xyz2,              // Coordinates of mon type 1 and 2
                                        double *mu1, double *mu2,                // Dipoles of mon type 1 and 2
                                        size_t mon1_index,                       // Mon 1 index
                                        size_t mon2_index_start,                 // Mon 2 initial index
                                        size_t mon2_index_end,                   // Mon 2 final index
                                        size_t nmon1, size_t nmon2,              // # monomers of types 1 and 2
                                        size_t site_i, size_t site_j,            // Site # i of mon1 and # j of mon 2
                                        double aDD,                              // Thole damping aDD (dipole - dipole)
                                        double *Efd2,                            // Electric field on Mon 2
                                        double *Efdx_mon1,                       // Output electric field on X for Mon 1
                                        double *Efdy_mon1,                       // Output electric field on Y for Mon 1
                                        double *Efdz_mon1,                       // Output electric field on Z for Mon 1
                                        PrecomputedInfo& precomputedInformation, // Contains precomputed coordinate-dependant calculations
                                        int mt1, int mt2, int m1, int i, int j); 
    /*
    *  This function finds the all monomers of type 2 which are within a a twobody_cutoff distance from monomer 1.
    *  It saves the indices of these monomers to bool_indices, which other electrostatics functions will use
    *  to ensure calculations are only done on monomers are which are within the cutoff.
    */
    void FindMonomersWithinCutoff(size_t *bool_indices, 
                                double *xyz1, double *xyz2,              // Coordinates of mon type 1 and 2
                                size_t m2init, 
                                size_t nmon1, size_t nmon2,              // # monomers of types 1 and 2
                                bool use_pbc,                            // Whether to enforce periodic boundary conditions
                                std::vector<double> &box,                // The lattice vectors
                                std::vector<double> &box_inverse,        // The inverse lattice vectors
                                double cutoff,                           // The real space cutoff for pairs
                                size_t site_i, 
                                size_t site_j, 
                                size_t mon1_index,                       // Monomer 1 index
                                bool use_ghost,                          // should we use ghost monomers
                                const std::vector<size_t> &islocal, 
                                const size_t isl1_offset,
                                const size_t isl2_offset);
    
    /*
    * This function is a subroutine of PrecomputeDipoleIterationsInformation (in electrostatics.cpp).
    * It calculates ts2x, ts2y, ts2z, rijx, rijy, rijz, slr3 and stores them in precomputedInformation.
    */
    void CalcPrecomputedDipoleElec(double *xyz1, double *xyz2,              // Coordinates of mon type 1 and 2
                                    size_t mon1_index,                       // Mon 1 index
                                    size_t mon2_index_start,                 // Mon 2 initial index
                                    size_t mon2_index_end,                   // Mon 2 final index
                                    size_t nmon1, size_t nmon2,              // # monomers of types 1 and 2
                                    size_t site_i, size_t site_j,            // Site # i of mon1 and # j of mon 2
                                    double Asqsqi,                           // (polfac[i] * polfac[j])^4 inverted
                                    double aDD,                              // Thole damping aDD (dipole - dipole)
                                    double ewald_alpha,                      // Ewald attenuation paramter
                                    bool use_pbc,                            // Whether to enforce periodic boundary conditions
                                    const std::vector<double> &box,          // The lattice vectors
                                    const std::vector<double> &box_inverse,  // The inverse lattice vectors
                                    double cutoff,                           // The real space cutoff for pairs
                                    bool use_ghost,                          // use ghost monomers
                                    const std::vector<size_t> &islocal, 
                                    const size_t isl1_offset,
                                    const size_t isl2_offset,
                                    PrecomputedInfo& precomputedInformation, // Contains precomputed coordinate-dependant calculations
                                    int mt1, int mt2, int m1, int i, int j);
    ////////////////////////////////////////////////////////////////////////////////
    // GRADIENTS AND ADD DIPOLE CONTRIBUTIONS TO POTENTIAL /////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    // Computes the gradients and updates the potential on sites i and j to
    // account for the contribution of the dipoles to the potential phi
    // for a number of monomers # = mon2_index_end - mon2_index_start when
    // A=polfac[i] * polfac[j] > 0

    void CalcElecFieldGrads(double *xyz1, double *xyz2,      // Coordinates of mon type 1 and 2
                            double *chg1, double *chg2,      // Charges of mon type 1 and 2
                            double *mu1, double *mu2,        // Dipoles of mon type 1 and 2
                            size_t mon1_index,               // Mon 1 index
                            size_t mon2_index_start,         // Mon 2 initial index
                            size_t mon2_index_end,           // Mon 2 final index
                            size_t nmon1, size_t nmon2,      // # monomers of types 1 and 2
                            size_t site_i, size_t site_j,    // Site # i of mon1 and # j of mon 2
                            double aDD, double aCD,          // Thole damping aCC and aDD
                            double Asqsqi,                   // (polfac[i] * polfac[j])^4
                            double *grdx,                    // Output gradient of site i of mon1 in X
                            double *grdy,                    // Output gradient of site i of mon1 in Y
                            double *grdz,                    // Output gradient of site i of mon1 in Z
                            double *phi1,                    // Output field on site i of mon1
                            double *phi2,                    // Field on site j of mon2
                            double *grd2,                    // Gradient on site j of mon2
                            double elec_scale_factor,        // Scale factor for electrostatic (1=included, 0=excluded)
                            double ewald_alpha,              // Ewald attenuation paramter
                            bool use_pbc,                    // Whether to enforce periodic boundary conditions
                            const std::vector<double> &box,  // The lattice vectors
                            const std::vector<double> &box_inverse,  // The inverse lattice vectors
                            double cutoff,                           // The real space cutoff for pairs
                            bool use_ghost,                          // use ghost monomers
                            const std::vector<size_t> &islocal, const size_t isl1_offset, const size_t isl2_offset,
                            std::vector<double> *virial = 0  // the virial
    );

    // Optimized version

    void CalcElecFieldGrads_Optimized(double *xyz1, double *xyz2,      // Coordinates of mon type 1 and 2
                            double *chg1, double *chg2,      // Charges of mon type 1 and 2
                            double *mu1, double *mu2,        // Dipoles of mon type 1 and 2
                            size_t mon1_index,               // Mon 1 index
                            size_t mon2_index_start,         // Mon 2 initial index
                            size_t mon2_index_end,           // Mon 2 final index
                            size_t nmon1, size_t nmon2,      // # monomers of types 1 and 2
                            size_t site_i, size_t site_j,    // Site # i of mon1 and # j of mon 2
                            double aDD, double aCD,          // Thole damping aCC and aDD
                            double Asqsqi,                   // (polfac[i] * polfac[j])^4
                            double *grdx,                    // Output gradient of site i of mon1 in X
                            double *grdy,                    // Output gradient of site i of mon1 in Y
                            double *grdz,                    // Output gradient of site i of mon1 in Z
                            double *phi1,                    // Output field on site i of mon1
                            double *phi2,                    // Field on site j of mon2
                            double *grd2,                    // Gradient on site j of mon2
                            double elec_scale_factor,        // Scale factor for electrostatic (1=included, 0=excluded)
                            double ewald_alpha,              // Ewald attenuation paramter
                            bool use_pbc,                    // Whether to enforce periodic boundary conditions
                            const std::vector<double> &box,  // The lattice vectors
                            const std::vector<double> &box_inverse,  // The inverse lattice vectors
                            double cutoff,                           // The real space cutoff for pairs
                            bool use_ghost,                          // use ghost monomers
                            const std::vector<size_t> &islocal, const size_t isl1_offset, const size_t isl2_offset,
                            PrecomputedInfo& precomputedInformation,
                            std::vector<double> *virial = 0  // the virial
    );

    ////////////////////////////////////////////////////////////////////////////////

   private:
    // Maximum number of monomers that can be in system
    // For now, is set to the largest number. Later can be set to
    // the maximum number of monomers we will evaluate at once.
    size_t maxnmon;

    // Vectors that will be reused:
    std::vector<double> v0_;
    std::vector<double> v1_;
    std::vector<double> v2_;
    std::vector<double> v3_;
    std::vector<double> v4_;
    std::vector<double> v5_;
    std::vector<double> v6_;
    std::vector<double> v7_;
    std::vector<double> v8_;
    std::vector<double> v9_;
    std::vector<double> v10_;
    std::vector<double> v11_;
};

}  // namespace elec
#endif
