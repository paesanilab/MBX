#ifndef ELECTROSTATICS_H
#define ELECTROSTATICS_H

#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>
#include <memory>

#ifdef _OPENMP
# include <omp.h>
#endif

#include "bblock/sys_tools.h"
#include "tools/definitions.h"
#include "tools/constants.h"
#include "tools/math_tools.h"
#include "potential/electrostatics/gammq.h"
#include "potential/electrostatics/fields.h"
#include "potential/electrostatics/electrostatic_tensors.h"
#include "potential/electrostatics/electrostatic_tensors_short.h"

////////////////////////////////////////////////////////////////////////////////

namespace elec {
  class Electrostatics {
    public:
      Electrostatics();
      void Initialize(
        std::vector<double> &chg,
        std::vector<double> &chg_grad,
        std::vector<double> &polfac,
        std::vector<double> &pol,
        std::vector<double> &sys_xyz,
        std::vector<std::string> &mon_id,
        std::vector<size_t> &sites,
        std::vector<size_t> &first_ind,
        std::vector<std::pair<std::string,size_t> > &mon_type_count,
        bool do_grads = true,
        double tolerance = 1E-16,
        size_t maxit = 100,
        std::string dip_method = "iter");
      
      double GetElectrostatics(std::vector<double> &grad);

    private:
      void CalculatePermanentElecField();
      void CalculateDipolesIterative();
      void DipolesIterativeIteration();
      void CalculateDipolesCG();
      void DipolesCGIteration(std::vector<double> &in_v,
                              std::vector<double> &out_v);
      void CalculateDipolesAspc();
      void SetAspcParameters(size_t k);
      void CalculateDipoles();
      void CalculateElecEnergy();
      void CalculateGradients(std::vector<double> &grad);

      // Charges of each site. Order has to follow mon_type_count.
      std::vector<double> chg_;
      // Charges of each site. Order has to follow mon_type_count.
      std::vector<double> sys_chg_;
      // Gradients due to site dependent charges
      std::vector<double> sys_chg_grad_;
      // Polfacs of each site. For now assuming not site dependent.
      std::vector<double> polfac_;
      // Square root of pol of each site. Size 3N (3 per atom, are the same)
      std::vector<double> pol_sqrt_;
      // Polarizabilities of each site. For now assuming not site dependent.
      std::vector<double> pol_;
      // System xyz, not ordered XYZ. xyzxyz...(mon1)xyzxyz...(mon2) ...
      std::vector<double> sys_xyz_;
      // System xyz, ordered XYZ. xx..yy..zz(mon1) xx..yy..zz(mon2) ...
      std::vector<double> xyz_;
      // Name of the monomers (h2o, f...)
      std::vector<std::string> mon_id_;
      // Number of sites of each mon
      std::vector<size_t> sites_;
      // First index of each atom in system
      std::vector<size_t> first_ind_;
      // Vector that contains all different monomer types and the number of
      // monomers of each type.
      std::vector<std::pair<std::string,size_t> > mon_type_count_;
      // Tolerance in the iterative calculation of the dipoles
      // Tolerance refers to the maximum squared difference overall the dipoles
      double tolerance_;
      // Maximum number of iterations allowed in the iterative dipole calculation
      size_t maxit_;
      // Bool that if true will perform the gradients calculation.
      bool do_grads_;
      // Gradients in the original order (same as xyz). This is the vector in
      // which the electrostatics gradients will be added.
      std::vector<double> sys_grad_;
      // Gradients
      std::vector<double> grad_;
      // Electric potential on each site with sys order
      std::vector<double> sys_phi_;
      // Electric potential on each site
      std::vector<double> phi_;
      // Permanent electric field with sys order
      std::vector<double> sys_Efq_;
      // Permanent electric field
      std::vector<double> Efq_;
      // Induced electric field with sys_order
      std::vector<double> sys_Efd_;
      // Induced electric field
      std::vector<double> Efd_;
      // Dipoles with sys order
      std::vector<double> sys_mu_;
      // Dipoles
      std::vector<double> mu_;
      // Dipole history for ASPC
      std::vector<double> mu_hist_;
      // Dipole predictor
      std::vector<double> mu_pred_;
      // B constants to use for ASPC
      std::vector<double> b_consts_aspc_;
      // omega used in ASPC
      double omega_aspc_;
      // Number of history steps stored
      size_t hist_num_aspc_;
      // Order of ASPC
      size_t k_aspc_; 
      // Total number of electrostatic sites
      size_t nsites_;
      // Thole dampings
      double aCC_, aCD_, aDD_;
      // Constants to be used later:
      // Gamma 3/4
      double g34_;
      // aCC ^ (1/4)
      double aCC1_4_; 
      // Max number of monomers
      size_t maxnmon_; 
      // Permanent electrostatics
      double Eperm_;
      // Induced electrostatics
      double Eind_;
      // Method for dipoles (ITERative, Conjugate Gradient, ASPC, INVersion)
      std::string dip_method_;
  };

////////////////////////////////////////////////////////////////////////////////

} //namespace elec

////////////////////////////////////////////////////////////////////////////////

#endif
