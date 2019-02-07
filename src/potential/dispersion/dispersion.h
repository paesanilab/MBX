#ifndef DISPERSION_NEW_H
#define DISPERSION_NEW_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "potential/dispersion/disptools.h"
#include "tools/definitions.h"
#include "bblock/sys_tools.h"
#include "tools/math_tools.h"

//#include "helpme.h"

namespace disp {

class Dispersion {
   public:
    Dispersion(){};
    ~Dispersion(){};

    void Initialize(const std::vector<double> C6_long_range, const std::vector<double> &sys_xyz,
                    const std::vector<std::string> &mon_id, const std::vector<size_t> &num_atoms,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count, const bool do_grads,
                    const std::vector<double> &box);

    double GetDispersion(std::vector<double> &grad);

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

   private:
    void ReorderData();
    void CalculateDispersion();

    // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    std::vector<double> sys_xyz_;
    // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
    std::vector<double> xyz_;
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

    // Total number of atoms
    size_t natoms_;
    // Max number of monomers
    size_t maxnmon_;
    // Dispersion energy
    double disp_energy_;
    // box of the system
    std::vector<double> box_;
    // inverted box of the system
    std::vector<double> box_inverse_;
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
};

}  // namespace disp

#endif
