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

#ifndef BUCKINGHAM_H
#define BUCKINGHAM_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "potential/buckingham/bucktools.h"
#include "tools/definitions.h"
#include "bblock/sys_tools.h"
#include "tools/math_tools.h"

//#include "helpme.h"

namespace buck {

class Buckingham {
   public:
    Buckingham(){};
    ~Buckingham(){};

    void Initialize(const std::vector<double> &sys_xyz,
                    const std::vector<std::string> &mon_id, const std::vector<size_t> &num_atoms,
                    const std::vector<std::pair<std::string, size_t> > &mon_type_count, 
                    const std::vector<size_t> force_ttm_for_idx,
                    const std::vector<size_t> &islocal_, const bool do_grads,
		    const std::vector<double> &box);

    double GetRepulsion(std::vector<double> &grad,std::vector<double> *virial = 0, bool use_ghost = 0);

    void SetNewParameters(const std::vector<double> &xyz, 
                          const std::vector<std::pair<std::string,std::string> > &buck_pairs, 
                          const std::vector<size_t> force_ttm_for_idx,
                          bool do_grads, const double cutoff,
                          const std::vector<double> &box);

    /**
     * @brief Sets the cutoff for dispersion interactions
     *
     * @param[in] cutoff New cutoff value
     */
    void SetCutoff(double cutoff);

   private:
    void ReorderData();
    void CalculateRepulsion(bool use_ghost = 0);
    void CalculateEnforcedRepulsion(bool use_ghost = 0);

    // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
    std::vector<double> sys_xyz_;
    // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
    std::vector<double> xyz_;
    // local/ghost descriptor for monomers
    std::vector<size_t> islocal_;
    // Name of the monomers (h2o, f...)
    std::vector<std::string> mon_id_;
    // Number of sites of each mon
    std::vector<size_t> num_atoms_;
    // monomers of each type.
    std::vector<std::pair<std::string, size_t> > mon_type_count_;

    // pairs that will use the buckingham
    std::vector<std::pair<std::string,std::string> > buck_pairs_;

    // Indexes of the monomers for which TTM is forced
    std::vector<size_t> force_ttm_for_idx_;

    // Bool that if true will perform the gradients calculation.
    bool do_grads_;

    // Gradients in the original order (same as xyz). This is the vector in
    // which the electrostatics gradients will be added.
    std::vector<double> sys_grad_;
    // Gradients
    std::vector<double> grad_;
    // Virial tensor
    std::vector<double> virial_;
    // virial calc variable
    bool calc_virial_;

    // Total number of atoms
    size_t natoms_;
    // Max number of monomers
    size_t maxnmon_;
    // Repulsion energy
    double rep_energy_;
    // box of the system
    std::vector<double> box_;
    // inverted box of the system
    std::vector<double> box_inverse_;
    // use pbc in the electrostatics calculation
    bool use_pbc_;
    // dispersion cutoff
    double cutoff_;
};

}  // namespace disp

#endif
