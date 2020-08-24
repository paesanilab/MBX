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

#ifndef DISPERSION_NEW_H
#define DISPERSION_NEW_H

#include <vector>

#if HAVE_MPI
#include <mpi.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#include "potential/dispersion/disptools.h"
#include "tools/definitions.h"
#include "bblock/sys_tools.h"
#include "tools/math_tools.h"

//#include "helpme.h"

#ifndef MPI_VERSION
// typedef struct ompi_communicator_t *MPI_Comm;
typedef int MPI_Comm;
#endif

namespace disp {

class Dispersion {
   public:
    Dispersion(){};
    ~Dispersion(){};

    void Initialize(const std::vector<double> C6_long_range, const std::vector<double> &sys_xyz,
                    const std::vector<std::string> &mon_id, const std::vector<size_t> &num_atoms,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count,
                    const std::vector<size_t> &islocal_, const bool do_grads, const std::vector<double> &box);

    void SetMPI(MPI_Comm world_, size_t proc_grid_x, size_t proc_grid_y, size_t proc_grid_z);

    double GetDispersion(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);
    double GetDispersionPME(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);
    double GetDispersionPMElocal(std::vector<double> &grad, std::vector<double> *virial = 0, bool use_ghost = 0);

    void SetNewParameters(const std::vector<double> &xyz, bool do_grads, const double cutoff,
                          const std::vector<double> &box);

    /**
     * @brief Sets the Ewald attenuation parameter (in units of 1/Angstrom)
     *
     * @param[in] cutoff New cutoff value
     */
    void setEwaldAlpha(const double alpha) { ewald_alpha_ = alpha; }

    /**
     * @brief Sets the PME grid density.
     *
     * @param[in] density the number of grid points per Angstrom in each dimension of the PME grid.
     */
    void SetEwaldGridDensity(double density) { pme_grid_density_ = density; }

    /**
     * @brief Sets the PME B-Spline order.
     *
     * @param[in] order the order of the B-Spline used to spread charges
     */
    void SetEwaldSplineOrder(int order) { pme_spline_order_ = order; }

    /**
     * @brief Sets the cutoff for dispersion interactions
     *
     * @param[in] cutoff New cutoff value
     */
    void SetCutoff(double cutoff);

    /**
     * Sets global box dimensions for PME solver; does not alter original PBC settings
     * @param[in] box is a 9 component vector of double with
     * the three main vectors of the cell: {v1x v1y v1z v2x v2y v2z v3x v3y v3z}
     */
    void SetBoxPMElocal(std::vector<double> box);

   private:
    void ReorderData();
    void CalculateDispersion(bool use_ghost = 0);
    void CalculateDispersionPME(bool use_ghost = 0);
    void CalculateDispersionPMElocal(bool use_ghost = 0);

    // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    std::vector<double> sys_xyz_;
    // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
    std::vector<double> xyz_;
    // local/ghost descriptor for monomers
    std::vector<size_t> islocal_;
    // local/ghost descriptor for atoms
    std::vector<size_t> islocal_atom_;
    // Name of the monomers (h2o, f...)
    std::vector<std::string> mon_id_;
    // Number of sites of each mon
    std::vector<size_t> num_atoms_;
    // Vector that contains the C6 used at long range
    std::vector<double> c6_long_range_;
    // Vector that contains the C6 used at long range
    std::vector<double> sys_c6_long_range_;
    // Vector that contains all different monomer types and the number of
    // monomers of each type.
    std::vector<std::pair<std::string, size_t> > mon_type_count_;

    // Bool that if true will perform the gradients calculation.
    bool do_grads_;

    // Gradients in the original order (same as xyz). This is the vector in
    // which the electrostatics gradients will be added.
    std::vector<double> sys_grad_;
    // Gradients
    std::vector<double> grad_;
    // Dispersion potential on each site with sys order
    std::vector<double> sys_phi_;
    // Dispersion potential on each site
    std::vector<double> phi_;
    // Virial tensor
    std::vector<double> virial_;
    // virial calc variable
    bool calc_virial_;
    // Total number of atoms
    size_t natoms_;
    // Max number of monomers
    size_t maxnmon_;
    // Dispersion energy
    double disp_energy_;
    // box of the system
    std::vector<double> box_;
    // box in ABCabc notation
    std::vector<double> box_ABCabc_;
    // inverted box of the system
    std::vector<double> box_inverse_;
    // box of the domain-decomposed system
    std::vector<double> box_PMElocal_;
    // use pbc in the electrostatics calculation
    bool use_pbc_;
    // dispersion cutoff
    double cutoff_;
    // ewald attenuation parameter in inverse angstroms
    double ewald_alpha_ = 0;
    // PME grid density
    double pme_grid_density_ = 0;
    // PME spline order
    int pme_spline_order_ = 0;
    // MPI initialized
    bool mpi_initialized_ = false;
    // MPI Communicator
    MPI_Comm world_;
    // proc_grid
    size_t proc_grid_x_;
    size_t proc_grid_y_;
    size_t proc_grid_z_;
};

}  // namespace disp

#endif
