#ifndef DISPERSION_NEW_H
#define DISPERSION_NEW_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "potential/dispersion/disptools_new.h"
#include "tools/definitions.h"
#include "bblock/sys_tools.h"

//#include "helpme.h"

namespace disp {

const double SQRTPI = sqrt(M_PI);

std::vector<double> InvertUnitCell(const std::vector<double> &box) {
    double determinant = box[0] * (box[4] * box[8] - box[7] * box[5]) - box[1] * (box[3] * box[8] - box[5] * box[6]) +
                         box[2] * (box[3] * box[7] - box[4] * box[6]);

    double determinant_inverse = 1 / determinant;
    std::vector<double> box_inverse(9);
    box_inverse[0] = (box[4] * box[8] - box[7] * box[5]) * determinant_inverse;
    box_inverse[1] = (box[2] * box[7] - box[1] * box[8]) * determinant_inverse;
    box_inverse[2] = (box[1] * box[5] - box[2] * box[4]) * determinant_inverse;
    box_inverse[3] = (box[5] * box[6] - box[3] * box[8]) * determinant_inverse;
    box_inverse[4] = (box[0] * box[8] - box[2] * box[6]) * determinant_inverse;
    box_inverse[5] = (box[3] * box[2] - box[0] * box[5]) * determinant_inverse;
    box_inverse[6] = (box[3] * box[7] - box[6] * box[4]) * determinant_inverse;
    box_inverse[7] = (box[6] * box[1] - box[0] * box[7]) * determinant_inverse;
    box_inverse[8] = (box[0] * box[4] - box[3] * box[1]) * determinant_inverse;
    return box_inverse;
}

class Dispersion {
    public:

    Dispersion();
    ~Dispersion();
    
    void Initialize(const std::vector<double> C6_long_range, const std::vector<double> &sys_xyz, const std::vector<std::string> &mon_id,
                    const std::vector<size_t> &sites, const std::vector<size_t> &first_ind,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count, const bool do_grads, const std::vector<double> &box);

    double GetDispersion(std::vector<double> &grad);

    void SetNewParameters(const std::vector<double> &xyz, bool do_grads, const double cutoff, const std::vector<double> &box);

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
    // First index of each atom in system
    std::vector<size_t> first_ind_;
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
    // use pbc in the electrostatics calculation
    bool use_pbc_;
    // dispersion cutoff
    double cutoff_;
    // ewald attenuation parameter in inverse angstroms

};


} // namespace disp



#endif
