#include "potential/electrostatics/electrostatics.h"

//#define DEBUG
//#define TIMING
//#define PRINT_TERMS

#ifdef DEBUG
#  include <iostream>
#endif
#ifdef TIMING
#  include <chrono>
#  include <iostream>
#endif

namespace elec {

  Electrostatics::Electrostatics() { };
  void Electrostatics::Initialize(
        std::vector<double> &chg,
        std::vector<double> &chg_grad,
        std::vector<double> &polfac,
        std::vector<double> &pol,
        std::vector<double> &sys_xyz,
        std::vector<std::string> &mon_id,
        std::vector<size_t> &sites,
        std::vector<size_t> &first_ind,
        std::vector<std::pair<std::string,size_t> > &mon_type_count,
        bool do_grads,
        double tolerance,
        size_t maxit) {

    // Copy System data in electrostatics
    sys_chg_ = chg;
    sys_chg_grad_ = chg_grad;
    polfac_ = polfac;
    pol_ = pol;
    sys_xyz_ = sys_xyz;
    mon_id_ = mon_id;
    sites_ = sites;
    first_ind_ = first_ind;
    mon_type_count_ = mon_type_count;
    do_grads_ = do_grads;
    tolerance_ = tolerance;
    maxit_ = maxit;

    // Initialize other variables
    nsites_ = sys_chg_.size();
    size_t nsites3 = nsites_ * 3;
    sys_phi_ = std::vector<double>(nsites_,0.0);
    phi_ = std::vector<double>(nsites_,0.0);
    sys_Efq_ = std::vector<double>(nsites3, 0.0);
    sys_Efd_ = std::vector<double>(nsites3, 0.0);
    Efq_ = std::vector<double>(nsites3, 0.0);
    Efd_ = std::vector<double>(nsites3, 0.0);
    sys_mu_ = std::vector<double>(nsites3, 0.0);
    mu_ = std::vector<double>(nsites3, 0.0);
    xyz_ = std::vector<double>(nsites3,0.0);
    grad_ = std::vector<double>(nsites3,0.0);
    sys_grad_ = std::vector<double>(nsites3,0.0);
    chg_ = std::vector<double>(nsites_,0.0);

    aCC_ = 0.4;
    aCD_ = 0.4;
    aDD_ = 0.055;
    g34_ = std::exp(gammln(0.75));
    aCC1_4_ = std::pow(aCC_,0.25);

////////////////////////////////////////////////////////////////////////////////
// DATA ORGANIZATION ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Reorganize xyz and charge
    // XYZ will be reorganized from x1 y1 z1 x2 ... to 
    // x1x2x3...y1y2y3...z1z2... for mon1, then x1x2x3...y1y2... for mon 2...
    // CHG will be reorganized from cA1cB1cC1... for mon_type 1 then mon2...
    // to cA1cA2...cB1cB2... for mon_type 1, then 2, ...
    // where A,B are the different sites, and 1,2 are the monomer index
    
    // Organize xyz so we have x1_1 x1_2 ... y1_1 y1_2...
    // where xN_M is read as coordinate x of site N of monomer M
    // for the first monomer type. Then follows the second, and so on.
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t m = 0; m < nmon; m++) {
        size_t mns = m*ns;
        size_t mns3 = mns*3;
        for (size_t i = 0; i < ns; i++) {
          size_t inmon = i*nmon;
          size_t inmon3 = 3*inmon;
          xyz_[inmon3 + m + fi_crd] =
                 sys_xyz_[fi_crd + mns3 + 3*i];
          xyz_[inmon3 + m + fi_crd + nmon] =
                 sys_xyz_[fi_crd + mns3 + 3*i + 1];
          xyz_[inmon3 + m + fi_crd + nmon2] =
                 sys_xyz_[fi_crd + mns3 + 3*i + 2];
          chg_[fi_sites + m + inmon] =
                 sys_chg_[fi_sites + mns + i];
        }
      }
      fi_mon += nmon;
      fi_sites += nmon*ns;
      fi_crd += nmon*ns*3;
    }
    
  }

////////////////////////////////////////////////////////////////////////////////
// PERMANENT ELECTRIC FIELD ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  void Electrostatics::CalculatePermanentElecField() {

    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#   ifdef _OPENMP
#     pragma omp parallel // omp_get_num_threads() needs to be inside 
                          // parallel region to get number of threads
      {
        if (omp_get_thread_num() == 0)
          nthreads = omp_get_num_threads();
      }
#   endif

    // This part looks at sites inside the same monomer
    // Reset first indexes
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      size_t nmon2 = 2*nmon;
 
      // Obtain excluded pairs for monomer type mt
      systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);

      // Loop over each pair of sites
      for (size_t i = 0; i < ns -1; i++) {
        size_t inmon = i * nmon;
        size_t inmon3 = inmon * 3;
        for (size_t j = i + 1; j < ns; j++) {

          // Continue only if i and j are not bonded
          bool is12 = systools::IsExcluded(exc12, i, j);
          bool is13 = systools::IsExcluded(exc13, i, j);
          bool is14 = systools::IsExcluded(exc14, i, j);
          if (is12 || is13 || is14) continue;

          // Get a1a2 and check if is not 0.
          double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
          if (A > constants::EPS) {
            A = std::pow(A,1.0/6.0);
            double Ai = 1/A;
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcPermanentElecFieldWithPolfacNonZero(
                        xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                        chg_.data() + fi_sites, chg_.data() + fi_sites, 
                        m, m, m+1, nmon, nmon, i, j, Ai, Asqsq,
                        aCC_, aCC1_4_, g34_, &ex, &ey, &ez, &phi1, 
                        phi_.data() + fi_sites, Efq_.data() + fi_crd);
              phi_[fi_sites + inmon + m] += phi1;
              Efq_[fi_crd + inmon3 + m] += ex; 
              Efq_[fi_crd + inmon3 + nmon + m] += ey;
              Efq_[fi_crd + inmon3 + nmon2 + m] += ez; 
            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcPermanentElecFieldWithPolfacZero(
                        xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                        chg_.data() + fi_sites, chg_.data() + fi_sites,
                        m, m, m+1, nmon, nmon, i, j,
                        &ex, &ey, &ez, &phi1,
                        phi_.data() + fi_sites, Efq_.data() + fi_crd);
              phi_[fi_sites + inmon + m] += phi1;
              Efq_[fi_crd + inmon3 + m] += ex;
              Efq_[fi_crd + inmon3 + nmon + m] += ey;
              Efq_[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          }
        }
      }

      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }
      
    // Sites corresponding to different monomers
    // Declaring first indexes
    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;

    // Loop over all monomer types
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
      size_t ns1 = sites_[fi_mon1];
      size_t nmon1 = mon_type_count_[mt1].second;
      size_t nmon12 = nmon1 * 2;
      fi_mon2 = fi_mon1;
      fi_sites2 = fi_sites1;
      fi_crd2 = fi_crd1;
      
      // For each monomer type mt1, loop over all the other monomer types
      // mt2 >= mt1 to avoid double counting
      for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
        size_t ns2 = sites_[fi_mon2];
        size_t nmon2 = mon_type_count_[mt2].second;
        
        // Check if monomer types 1 and 2 are the same
        // If so, same monomer won't be done, since it has been done in
        // previous loop.
        bool same = (mt1 == mt2);

        // TODO add neighbour list here
        // Loop over all pair of sites

        std::vector<std::shared_ptr<ElectricFieldHolder> > field_pool;
        std::vector<std::vector<double> > Efq_1_pool;
        std::vector<std::vector<double> > Efq_2_pool;
        std::vector<std::vector<double> > phi_1_pool;
        std::vector<std::vector<double> > phi_2_pool;
        for (size_t i = 0; i < nthreads; i++) { 
           field_pool.push_back(
             std::make_shared<ElectricFieldHolder>(maxnmon));
           Efq_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
           Efq_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
           phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
           phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
        }

#       ifdef _OPENMP
#         pragma omp parallel for schedule(dynamic)
#       endif
        for (size_t m1 = 0; m1 < nmon1; m1++) {
          int rank = 0;
#         ifdef _OPENMP
            rank = omp_get_thread_num();
#         endif          
          std::shared_ptr<ElectricFieldHolder> local_field
              = field_pool[rank];
          size_t m2init = same ? m1 + 1 : 0;
          double ex_thread = 0.0;
          double ey_thread = 0.0;
          double ez_thread = 0.0;
          double phi1_thread = 0.0;
          for (size_t i = 0; i < ns1; i++) {
            size_t inmon1 = i * nmon1;
            size_t inmon13 = inmon1 * 3;
            for (size_t j = 0; j < ns2; j++) {

              // Check if A = 0 and call the proper field calculation
              double A = polfac_[fi_sites1 + i] * polfac_[fi_sites2 + j];
              if (A > constants::EPS) {
                A = std::pow(A,1.0/6.0);
                double Ai = 1/A;
                double Asqsq = A*A*A*A;
                local_field->CalcPermanentElecFieldWithPolfacNonZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        chg_.data() + fi_sites1, chg_.data() + fi_sites2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, Ai, Asqsq,
                        aCC_, aCC1_4_, g34_, 
                        &ex_thread, &ey_thread, &ez_thread, &phi1_thread, 
                        phi_2_pool[rank].data(), Efq_2_pool[rank].data());
                phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                Efq_1_pool[rank][inmon13 + m1] += ex_thread;       
                Efq_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                Efq_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
              } else {
                elec_field.CalcPermanentElecFieldWithPolfacZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        chg_.data() + fi_sites1, chg_.data() + fi_sites2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j,
                        &ex_thread, &ey_thread, &ez_thread, &phi1_thread,
                        phi_2_pool[rank].data(), Efq_2_pool[rank].data());  
                phi_1_pool[rank][inmon1 + m1] += phi1_thread;
                Efq_1_pool[rank][inmon13 + m1] += ex_thread;
                Efq_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                Efq_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
              }
            }
          } 
        }
        // Compress data in Efq and phi
        for (size_t rank = 0; rank < nthreads; rank++) {
          size_t kend1 = Efq_1_pool[rank].size();
          size_t kend2 = Efq_2_pool[rank].size();
          for (size_t k = 0; k < kend1; k++) {
            Efq_[fi_crd1 + k] += Efq_1_pool[rank][k];
          }
          for (size_t k = 0; k < kend2; k++) {
            Efq_[fi_crd2 + k] += Efq_2_pool[rank][k];
          }
          kend1 = phi_1_pool[rank].size();
          kend2 = phi_2_pool[rank].size();
          for (size_t k = 0; k < kend1; k++) {
            phi_[fi_sites1 + k] += phi_1_pool[rank][k];
          }
          for (size_t k = 0; k < kend2; k++) {
            phi_[fi_sites2 + k] += phi_2_pool[rank][k];
          }
        }
        // Update first indexes
        fi_mon2 += nmon2;
        fi_sites2 += nmon2 * ns2;   
        fi_crd2 += nmon2 * ns2 * 3;
      }
      // Update first indexes
      fi_mon1 += nmon1;
      fi_sites1 += nmon1 * ns1;
      fi_crd1 += nmon1 * ns1 * 3;
    }

  }

////////////////////////////////////////////////////////////////////////////////
// DIPOLE ELECTRIC FIELD ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  void Electrostatics::CalculateDipoles() {
    // Permanent electric field is computed
    // Now start computation of dipole through iteration
    double eps = 1.0E+50;
    std::vector<double> mu_old(3*nsites_,0.0);
    size_t iter = 0;

    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;

    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#   ifdef _OPENMP
#     pragma omp parallel // omp_get_num_threads() needs to be inside 
                          // parallel region to get number of threads
      {
        if (omp_get_thread_num() == 0)
          nthreads = omp_get_num_threads();
      }
#   endif

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;
  
    while (true) {

      double max_eps = 0.0;
      //  Get new dipoles and check max difference
      size_t fi_mon = 0;
      size_t fi_crd = 0;
      size_t fi_sites = 0;
      double alpha = 0.8;
      for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = nmon*2;
        for (size_t i = 0; i < ns; i++) {
          // TODO assuming pol not site dependant
          double p = pol_[fi_sites + i];
          size_t inmon3 = 3*i*nmon;
          for (size_t m = 0; m < nmon; m++) {
            mu_old[fi_crd + inmon3 + m] = 
                         mu_[fi_crd + inmon3 + m];
            mu_old[fi_crd + inmon3 + nmon + m] = 
                         mu_[fi_crd + inmon3 + nmon + m];
            mu_old[fi_crd + inmon3 + nmon2 + m] = 
                         mu_[fi_crd + inmon3 + nmon2 + m];
            mu_[fi_crd + inmon3 + m] = alpha * p 
                       * (Efq_[fi_crd + inmon3 + m] 
                       +  Efd_[fi_crd + inmon3 + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + m];  
            mu_[fi_crd + inmon3 + nmon + m] = alpha * p 
                       * (Efq_[fi_crd + inmon3 + nmon + m] 
                       +  Efd_[fi_crd + inmon3 + nmon + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + nmon + m];
            mu_[fi_crd + inmon3 + nmon2 + m] = alpha * p 
                       * (Efq_[fi_crd + inmon3 + nmon2 + m] 
                       +  Efd_[fi_crd + inmon3 + nmon2 + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + nmon2 + m];  
          }
          
          // Check for max epsilon
          for (size_t m = 0; m < nmon; m++) {
            double tmpeps = (mu_[fi_crd + inmon3 + m] 
                           - mu_old[fi_crd + inmon3 + m])
                           *(mu_[fi_crd + inmon3 + m] 
                           - mu_old[fi_crd + inmon3 + m])
                           +(mu_[fi_crd + inmon3 + nmon + m]
                           - mu_old[fi_crd + inmon3 + nmon + m])
                           *(mu_[fi_crd + inmon3 + nmon + m] 
                           - mu_old[fi_crd + inmon3 + nmon + m])
                           +(mu_[fi_crd + inmon3 + nmon2 + m]
                           - mu_old[fi_crd + inmon3 + nmon2 + m])
                           *(mu_[fi_crd + inmon3 + nmon2 + m] 
                           - mu_old[fi_crd + inmon3 + nmon2 + m]);
            if (tmpeps > max_eps) max_eps = tmpeps;
          }
        }  
        fi_mon += nmon;
        fi_sites += nmon*ns;
        fi_crd += nmon*ns*3;
      }

      // Check if convergence achieved
      if (max_eps < tolerance_) 
        break;
      // Check if epsilon is increasing
      if (max_eps > eps && iter > 10) {
        // Exit with error
        std::cerr << "Dipoles diverged" << std::endl;
        std::exit(EXIT_FAILURE);
      } 
      eps = max_eps;

      // If not, check iter number
      if (iter > maxit_) {
        // Exit with error
        std::cerr << "Max number of iterations reached" << std::endl;
        std::exit(EXIT_FAILURE);
      }
      iter++;
      //std::cout << iter << std::endl;
      
      std::fill(Efd_.begin(), Efd_.end(), 0.0);

      double aDD = 0.0;

      // Recalculate Electric field due to dipoles
      // Sites on the same monomer
      fi_mon = 0;
      fi_sites = 0;
      fi_crd = 0;

      for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
        size_t ns = sites_[fi_mon];
//        TODO: Check why this makes shit fail
//        if (ns == 1) continue;
        size_t nmon = mon_type_count_[mt].second;
        size_t nmon2 = 2*nmon;
        // Get excluded pairs for this monomer
        systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);
        for (size_t i = 0; i < ns-1 ; i++) {
          size_t inmon3 = 3*i*nmon;
          for (size_t j = i+1; j < ns; j++) {
            // Set the proper aDD
            bool is12 = systools::IsExcluded(exc12, i, j);
            bool is13 = systools::IsExcluded(exc13, i, j);
            bool is14 = systools::IsExcluded(exc14, i, j);
            aDD = systools::GetAdd(is12, is13, is14, mon_id_[fi_mon]);

            double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
            if (A > constants::EPS) {
              A = std::pow(A, 1.0/6.0);
              double Asqsq = A*A*A*A;
              for (size_t m = 0; m < nmon; m++) {
                // TODO. Slowest function
                elec_field.CalcDipoleElecFieldWithPolfacNonZero(
                          xyz_.data() + fi_crd, xyz_.data() + fi_crd, 
                          mu_.data() + fi_crd, mu_.data() + fi_crd, m, m, m + 1,
                          nmon, nmon, i, j, Asqsq,
                          aDD, Efd_.data() + fi_crd, &ex, &ey, &ez);
                Efd_[fi_crd + inmon3 + m] += ex;
                Efd_[fi_crd + inmon3 + nmon + m] += ey;
                Efd_[fi_crd + inmon3 + nmon2 + m] += ez;
              }
            } else {
              for (size_t m = 0; m < nmon; m++) {
                elec_field.CalcDipoleElecFieldWithPolfacZero(
                          xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                          mu_.data() + fi_crd, mu_.data() + fi_crd, m, m, m + 1,
                          nmon, nmon, i, j, Efd_.data() + fi_crd, &ex, &ey, &ez);
                Efd_[fi_crd + inmon3 + m] += ex;
                Efd_[fi_crd + inmon3 + nmon + m] += ey;
                Efd_[fi_crd + inmon3 + nmon2 + m] += ez;
              }
            }
          }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
      }

      size_t fi_mon1 = 0;
      size_t fi_sites1 = 0;
      size_t fi_mon2 = 0;
      size_t fi_sites2 = 0;
      size_t fi_crd1 = 0;
      size_t fi_crd2 = 0;
      // aDD intermolecular is always 0.055
      aDD = 0.055;
      for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
        size_t ns1 = sites_[fi_mon1];
        size_t nmon1 = mon_type_count_[mt1].second;
        size_t nmon12 = 2 * nmon1;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
          size_t ns2 = sites_[fi_mon2];
          size_t nmon2 = mon_type_count_[mt2].second;
          bool same = (mt1 == mt2);
          // TODO add neighbour list here
          // Prepare for parallelization
          std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
          std::vector<std::vector<double>> Efd_1_pool;
          std::vector<std::vector<double>> Efd_2_pool;
          for (size_t i = 0; i < nthreads; i++) { 
             field_pool.push_back(
               std::make_shared<ElectricFieldHolder>(maxnmon));
             Efd_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
             Efd_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
          }

          // Parallel loop
#         ifdef _OPENMP
#           pragma omp parallel for schedule(dynamic)
#         endif
          for (size_t m1 = 0; m1 < nmon1; m1++) {
            int rank = 0;
#           ifdef _OPENMP
              rank = omp_get_thread_num();
#           endif
            std::shared_ptr<ElectricFieldHolder> local_field 
              = field_pool[rank];
            size_t m2init = same ? m1 + 1 : 0;
            double ex_thread = 0.0;
            double ey_thread = 0.0;
            double ez_thread = 0.0;
            for (size_t i = 0; i < ns1; i++) {
              size_t inmon13 = 3 * nmon1 * i;
              for (size_t j = 0; j < ns2; j++) {
                double A = polfac_[fi_sites1 + i] * polfac_[fi_sites2 + j];
                if (A > constants::EPS) {
                  A = std::pow(A,1.0/6.0);
                  double Asqsq = A*A*A*A;
                  local_field->CalcDipoleElecFieldWithPolfacNonZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        mu_.data() + fi_crd1, mu_.data() + fi_crd2, 
                        m1, m2init, nmon2,
                        nmon1, nmon2, i, j, Asqsq,
                        aDD, Efd_2_pool[rank].data(), 
                        &ex_thread, &ey_thread, &ez_thread);
                  Efd_1_pool[rank][inmon13 + m1] += ex_thread;
                  Efd_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                  Efd_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                } else {
                  local_field->CalcDipoleElecFieldWithPolfacZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        mu_.data() + fi_crd1, mu_.data() + fi_crd2, 
                        m1, m2init, nmon2, nmon1, nmon2, 
                        i, j, Efd_2_pool[rank].data(), 
                        &ex_thread, &ey_thread, &ez_thread);
                  Efd_1_pool[rank][inmon13 + m1] += ex_thread;
                  Efd_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                  Efd_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                }
              }
            }
          }
          
          // Compress data in Efd
          for (size_t rank = 0; rank < nthreads; rank++) {
            size_t kend1 = Efd_1_pool[rank].size();
            size_t kend2 = Efd_2_pool[rank].size();
            for (size_t k = 0; k < kend1; k++) {
              Efd_[fi_crd1 + k] += Efd_1_pool[rank][k];
            }
            for (size_t k = 0; k < kend2; k++) {
              Efd_[fi_crd2 + k] += Efd_2_pool[rank][k];
            }
          }
          // Update first indexes
          fi_mon2 += nmon2;
          fi_sites2 += nmon2 * ns2;
          fi_crd2 += nmon2 * ns2 * 3;
        }
        // Update first indexes
        fi_mon1 += nmon1;
        fi_sites1 += nmon1 * ns1;
        fi_crd1 += nmon1 * ns1 * 3;
      }
    }
  }

  void Electrostatics::CalculateElecEnergy() {
    Eperm_ = 0.0;
    for (size_t i = 0; i < nsites_; i++)
      Eperm_ += phi_[i] * chg_[i];
    Eperm_ *= 0.5;

    // Induced Electrostatic energy (chg-dip, dip-dip, pol)
    Eind_ = 0.0;
    for (size_t i = 0; i < 3*nsites_; i++) 
      Eind_ -= mu_[i] * Efq_[i];
    Eind_ *= 0.5;

#   ifdef PRINT_TERMS
    std::cerr << "E_ind = " << Eind_ << "   E_perm = " << Eperm << std::endl;
#   endif
  }

  void Electrostatics::CalculateGradients(std::vector<double> &grad) {
    // Reset grad
    grad_ = std::vector<double>(3*nsites_,0.0);
    
    // Max number of monomers
    size_t maxnmon = mon_type_count_.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;
#   ifdef _OPENMP
#     pragma omp parallel // omp_get_num_threads() needs to be inside 
                          // parallel region to get number of threads
      {
        if (omp_get_thread_num() == 0)
          nthreads = omp_get_num_threads();
      }
#   endif

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;
    
    // Auxiliary variables
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    double aDD = 0.0;
    size_t nsites3 = 3*nsites_;

    // Chg-Chg interactions
    size_t fi_mon = 0;
    size_t fi_sites = 0;
    size_t fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t i = 0; i < ns ; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        for (size_t m = 0; m < nmon; m++) {
          grad_[fi_crd + inmon3 + m] -= 
            chg_[fi_sites + inmon + m]*Efq_[fi_crd + inmon3 + m];
          grad_[fi_crd + inmon3 + nmon + m] -= 
            chg_[fi_sites + inmon + m]*Efq_[fi_crd + inmon3 + nmon + m];
          grad_[fi_crd + inmon3 + nmon2 + m] -= 
            chg_[fi_sites + inmon + m]*Efq_[fi_crd + inmon3 + nmon2 + m];
        }
      }
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    // This will only be used for intramonomer to not get chg-dip grad
    std::vector<double> zeros(nsites_,0.0);

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;

    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      size_t nmon2 = nmon*2;
      systools::GetExcluded(mon_id_[fi_mon], exc12, exc13, exc14);
      for (size_t i = 0; i < ns - 1 ; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        for (size_t j = i + 1; j < ns; j++) {
          // Set the proper aDD
          bool is12 = systools::IsExcluded(exc12, i, j);
          bool is13 = systools::IsExcluded(exc13, i, j);
          bool is14 = systools::IsExcluded(exc14, i, j);
          // Don't do charge-dipole and modify phi if pair is excluded
          // TODO check this for distances more than 1-4
          // TODO careful. YOu are not adding phi1
          double * phi_mod = (is12 || is13 || is14) ?
                              0 : phi_.data() + fi_sites; 
          aDD = systools::GetAdd(is12, is13, is14, mon_id_[fi_mon]);
          double A = polfac_[fi_sites + i] * polfac_[fi_sites + j];
          if (A > constants::EPS) {
            A = std::pow(A, 1.0/6.0);
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcElecFieldGradsWithPolfacNonZero(
                        xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                        zeros.data(), zeros.data(),
                        mu_.data() + fi_crd, mu_.data() + fi_crd,
                        m, m, m+1, nmon, nmon, i, j, aDD, aCD_, Asqsq,
                        &ex, &ey, &ez, &phi1, phi_mod,
                        grad_.data() + fi_crd);
              grad_[fi_crd + inmon3 + m] += ex;
              grad_[fi_crd + inmon3 + nmon + m] += ey;
              grad_[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcElecFieldGradsWithPolfacZero(
                        xyz_.data() + fi_crd, xyz_.data() + fi_crd,
                        zeros.data(), zeros.data(),
                        mu_.data() + fi_crd, mu_.data() + fi_crd,
                        m, m, m+1, nmon, nmon, i, j, 
                        &ex, &ey, &ez, &phi1, phi_mod,
                        grad_.data() + fi_crd);
              grad_[fi_crd + inmon3 + m] += ex;
              grad_[fi_crd + inmon3 + nmon + m] += ey;
              grad_[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          }
        }
      }
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count_.size(); mt1++) {
      size_t ns1 = sites_[fi_mon1];
      size_t nmon1 = mon_type_count_[mt1].second;
      size_t nmon12 = nmon1*2;
      fi_mon2 = fi_mon1;
      fi_sites2 = fi_sites1;
      fi_crd2 = fi_crd1;
      for (size_t mt2 = mt1; mt2 < mon_type_count_.size(); mt2++) {
        size_t ns2 = sites_[fi_mon2];
        size_t nmon2 = mon_type_count_[mt2].second;
        bool same = (mt1 == mt2);
        // TODO add neighbour list here
        std::vector<std::shared_ptr<ElectricFieldHolder>> field_pool;
        std::vector<std::vector<double>> grad_1_pool;
        std::vector<std::vector<double>> grad_2_pool;
        std::vector<std::vector<double>> phi_1_pool;
        std::vector<std::vector<double>> phi_2_pool;
        for (size_t i = 0; i < nthreads; i++) { 
           field_pool.push_back(
             std::make_shared<ElectricFieldHolder>(maxnmon));
           grad_1_pool.push_back(std::vector<double>(nmon1 * ns1 * 3, 0.0));
           grad_2_pool.push_back(std::vector<double>(nmon2 * ns2 * 3, 0.0));
           phi_1_pool.push_back(std::vector<double>(nmon1 * ns1, 0.0));
           phi_2_pool.push_back(std::vector<double>(nmon2 * ns2, 0.0));
        }
#       ifdef _OPENMP
#         pragma omp parallel for schedule(dynamic)
#       endif
        for (size_t m1 = 0; m1 < nmon1; m1++) {
          int rank = 0;
#         ifdef _OPENMP
            rank = omp_get_thread_num();
#         endif
          std::shared_ptr<ElectricFieldHolder> local_field
              = field_pool[rank];
          size_t m2init = same ? m1 + 1 : 0;
          double ex_thread = 0.0;
          double ey_thread = 0.0;
          double ez_thread = 0.0;
          double phi1_thread = 0.0;
          for (size_t i = 0; i < ns1; i++) {
            size_t inmon1  = i*nmon1;
            size_t inmon13  = 3*inmon1;
            for (size_t j = 0; j < ns2; j++) {
              double A = polfac_[fi_sites1 + i] * polfac_[fi_sites2 + j];
              if (A > constants::EPS) {
                A = std::pow(A,1.0/6.0);
                double Asqsq = A*A*A*A;
                local_field->CalcElecFieldGradsWithPolfacNonZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        chg_.data() + fi_sites1, chg_.data() + fi_sites2,
                        mu_.data() + fi_crd1, mu_.data() + fi_crd2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, 
                        aDD, aCD_, Asqsq,
                        &ex_thread, &ey_thread, &ez_thread, &phi1_thread, 
                        phi_2_pool[rank].data(), grad_2_pool[rank].data());
                grad_1_pool[rank][inmon13 + m1] += ex_thread;
                grad_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                grad_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                phi_1_pool[rank][inmon1 + m1] += phi1_thread;
              } else {
                local_field->CalcElecFieldGradsWithPolfacZero(
                        xyz_.data() + fi_crd1, xyz_.data() + fi_crd2,
                        chg_.data() + fi_sites1, chg_.data() + fi_sites2,
                        mu_.data() + fi_crd1, mu_.data() + fi_crd2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, 
                        &ex_thread, &ey_thread, &ez_thread, &phi1_thread, 
                        phi_2_pool[rank].data(), grad_2_pool[rank].data());
                grad_1_pool[rank][inmon13 + m1] += ex_thread;
                grad_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                grad_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                phi_1_pool[rank][inmon1 + m1] += phi1_thread;
              }
            }
          }
        }
        // Compress data in grad and phi
        for (size_t rank = 0; rank < nthreads; rank++) {
          size_t kend1 = grad_1_pool[rank].size();
          size_t kend2 = grad_2_pool[rank].size();
          for (size_t k = 0; k < kend1; k++) {
            grad_[fi_crd1 + k] += grad_1_pool[rank][k];
          }
          for (size_t k = 0; k < kend2; k++) {
            grad_[fi_crd2 + k] += grad_2_pool[rank][k];
          }
          kend1 = phi_1_pool[rank].size();
          kend2 = phi_2_pool[rank].size();
          for (size_t k = 0; k < kend1; k++) {
            phi_[fi_sites1 + k] += phi_1_pool[rank][k];
          }
          for (size_t k = 0; k < kend2; k++) {
            phi_[fi_sites2 + k] += phi_2_pool[rank][k];
          }
        }
        // Update first indexes
        fi_mon2 += nmon2;
        fi_sites2 += nmon2 * ns2;
        fi_crd2 += nmon2 * ns2 * 3;
      }
      // Update first indexes
      fi_mon1 += nmon1;
      fi_sites1 += nmon1 * ns1;
      fi_crd1 += nmon1 * ns1 * 3;
    }
    
////////////////////////////////////////////////////////////////////////////////
// REVERT DATA ORGANIZATION ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Reorganize field and potential to initial order
    std::vector<double> tmp1(nsites3,0.0);
    std::vector<double> tmp2(nsites_,0.0);
    std::vector<double> tmp3(nsites3,0.0);
    std::vector<double> tmp4(nsites3,0.0);
    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t m = 0; m < nmon; m++) {
        size_t mns = m*ns;
        size_t mns3 = mns*3;
        for (size_t i = 0; i < ns; i++) {
          size_t inmon = i*nmon;
          size_t inmon3 = 3*inmon;
          sys_Efq_[fi_crd + mns3 + 3*i] = Efq_[inmon3 + m + fi_crd];
          sys_Efq_[fi_crd + mns3 + 3*i + 1] = Efq_[inmon3 + m + fi_crd + nmon];
          sys_Efq_[fi_crd + mns3 + 3*i + 2] = Efq_[inmon3 + m + fi_crd + nmon2];

          sys_Efd_[fi_crd + mns3 + 3*i] = Efd_[inmon3 + m + fi_crd];
          sys_Efd_[fi_crd + mns3 + 3*i + 1] = Efd_[inmon3 + m + fi_crd + nmon];
          sys_Efd_[fi_crd + mns3 + 3*i + 2] = Efd_[inmon3 + m + fi_crd + nmon2];

          sys_mu_[fi_crd + mns3 + 3*i] = mu_[inmon3 + m + fi_crd];
          sys_mu_[fi_crd + mns3 + 3*i + 1] = mu_[inmon3 + m + fi_crd + nmon];
          sys_mu_[fi_crd + mns3 + 3*i + 2] = mu_[inmon3 + m + fi_crd + nmon2];

          sys_phi_[fi_sites + mns + i] = phi_[fi_sites + m + inmon];

          grad[fi_crd + mns3 + 3*i] += grad_[inmon3 + m + fi_crd];
          grad[fi_crd + mns3 + 3*i + 1] += grad_[inmon3 + m + fi_crd + nmon];
          grad[fi_crd + mns3 + 3*i + 2] += grad_[inmon3 + m + fi_crd + nmon2];
        }
      }
      fi_mon += nmon;
      fi_sites += nmon*ns;
      fi_crd += nmon*ns*3;
    }

////////////////////////////////////////////////////////////////////////////////
// REDISTRIBUTION OF THE GRADIENTS AND GRADIENTS DUE TO SITE-DEPENDENT CHARGES /
////////////////////////////////////////////////////////////////////////////////

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count_.size(); mt++) {
      size_t ns = sites_[fi_mon];
      size_t nmon = mon_type_count_[mt].second;
      std::string id = mon_id_[fi_mon];

      // Redistribute gradients
      systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, grad);

      // Gradients due to position dependant charges
      systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites,
                               sys_phi_, grad, sys_chg_grad_);
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }
  }

  double Electrostatics::GetElectrostatics(std::vector<double> &grad) {
    CalculatePermanentElecField();
    CalculateDipoles();
    CalculateElecEnergy();
    if (do_grads_) CalculateGradients(grad);
    return Eperm_ + Eind_;
  }

} // namespace elec
