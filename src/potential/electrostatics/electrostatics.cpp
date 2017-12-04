#include "potential/electrostatics/electrostatics.h"

//#define DEBUG
//#define TIMING
#ifdef DEBUG
#  include <iostream>
#endif
#ifdef TIMING
#  include <chrono>
#  include <iostream>
#endif

namespace elec {

  double Electrostatics(const std::vector<double> orig_chg,               
    const std::vector<double> chg_grd, 
    const std::vector<double> polfac, 
    const std::vector<double> pol, 
    const std::vector<double> orig_xyz, 
    const std::vector<std::string> mon_id, 
    const std::vector<size_t> sites, 
    const std::vector<size_t> first_ind,
    const std::vector<std::pair<std::string,size_t>> mon_type_count, 
    const double tolerance, const size_t maxit, const bool do_grads, 
    std::vector<double> &orig_grd) {

#   ifdef TIMING
    auto t1 = std::chrono::high_resolution_clock::now();
#   endif

    // Thole damping declarations
    // aCC and aCD are fixed for all systems, any site, to 0.4
    const double aCC = 0.4;
    const double aCC1_4 = std::pow(aCC,0.25);
    const double aCD = 0.4;
    double aDD = 0.055;

    // Constants that will be used later
    const double g34 = std::exp(gammln(0.75));

    // System properties and sizes
    const size_t nsites = orig_chg.size();
    const size_t nsites3 = 3*nsites;

    // Electric fields and potential
    std::vector<double> Efq(nsites3,0.0);
    std::vector<double> Efd(nsites3,0.0);
    std::vector<double> phi(nsites,0.0);
    double ex = 0.0;
    double ey = 0.0;
    double ez = 0.0;
    double phi1 = 0.0;

    // Dipole vector
    std::vector<double> mu(nsites3,0.0);

    // Max number of monomers
    size_t maxnmon = mon_type_count.back().second;
    ElectricFieldHolder elec_field(maxnmon);

    // Parallelization
    size_t nthreads = 1;

////////////////////////////////////////////////////////////////////////////////
// DATA ORGANIZATION ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Organize xyz so we have x1_1 x1_2 ... y1_1 y1_2...
    // where xN_M is read as coordinate x of site N of monomer M
    // for the first monomer type. Then follows the second, and so on.
    std::vector<double> xyz(nsites3,0.0);
    std::vector<double> grd(nsites3,0.0);
    std::vector<double> chg(nsites,0.0);
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t m = 0; m < nmon; m++) {
        size_t mns = m*ns;
        size_t mns3 = mns*3;
        for (size_t i = 0; i < ns; i++) {
          size_t inmon = i*nmon;
          size_t inmon3 = 3*inmon;
          xyz[inmon3 + m + fi_crd] = 
                 orig_xyz[fi_crd + mns3 + 3*i];
          xyz[inmon3 + m + fi_crd + nmon] = 
                 orig_xyz[fi_crd + mns3 + 3*i + 1];
          xyz[inmon3 + m + fi_crd + nmon2] = 
                 orig_xyz[fi_crd + mns3 + 3*i + 2];
          chg[fi_sites + m + inmon] = 
                 orig_chg[fi_sites + mns + i];
        }
      }
      fi_mon += nmon;
      fi_sites += nmon*ns;
      fi_crd += nmon*ns*3;
    }

#   ifdef TIMING
    auto t2 = std::chrono::high_resolution_clock::now();
#   endif
////////////////////////////////////////////////////////////////////////////////
// PERMANENT ELECTRIC FIELD ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // This part looks at sites inside the same monomer
    // Reset first indexes
    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

#   ifdef TIMING
    auto t3 = std::chrono::high_resolution_clock::now();
#   endif

    // Loop over each monomer type
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = 2*nmon;
 
      // Obtain excluded pairs for monomer type mt
      systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);

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
          double A = polfac[fi_sites + i] * polfac[fi_sites + j];
          if (A > constants::EPS) {
            A = std::pow(A,1.0/6.0);
            double Ai = 1/A;
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcPermanentElecFieldWithPolfacNonZero(
                        xyz.data() + fi_crd, xyz.data() + fi_crd,
                        chg.data() + fi_sites, chg.data() + fi_sites, 
                        m, m, m+1, nmon, nmon, i, j, Ai, Asqsq,
                        aCC, aCC1_4, g34, &ex, &ey, &ez, &phi1, 
                        phi.data() + fi_sites, Efq.data() + fi_crd);
              phi[fi_sites + inmon + m] += phi1;
              Efq[fi_crd + inmon3 + m] += ex; 
              Efq[fi_crd + inmon3 + nmon + m] += ey;
              Efq[fi_crd + inmon3 + nmon2 + m] += ez; 
            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcPermanentElecFieldWithPolfacZero(
                        xyz.data() + fi_crd, xyz.data() + fi_crd,
                        chg.data() + fi_sites, chg.data() + fi_sites,
                        m, m, m+1, nmon, nmon, i, j,
                        &ex, &ey, &ez, &phi1,
                        phi.data() + fi_sites, Efq.data() + fi_crd);
              phi[fi_sites + inmon + m] += phi1;
              Efq[fi_crd + inmon3 + m] += ex;
              Efq[fi_crd + inmon3 + nmon + m] += ey;
              Efq[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          }
        }
      }
      
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

#   ifdef TIMING
    auto t4 = std::chrono::high_resolution_clock::now();
#   endif

    // Sites corresponding to different monomers
    // Declaring first indexes
    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
    
    // Loop over all monomer types
    for (size_t mt1 = 0; mt1 < mon_type_count.size(); mt1++) {
      size_t ns1 = sites[fi_mon1];
      size_t nmon1 = mon_type_count[mt1].second;
      size_t nmon12 = nmon1 * 2;
      fi_mon2 = fi_mon1;
      fi_sites2 = fi_sites1;
      fi_crd2 = fi_crd1;
      
      // For each monomer type mt1, loop over all the other monomer types
      // mt2 >= mt1 to avoid double counting
      for (size_t mt2 = mt1; mt2 < mon_type_count.size(); mt2++) {
        size_t ns2 = sites[fi_mon2];
        size_t nmon2 = mon_type_count[mt2].second;
        
        // Check if monomer types 1 and 2 are the same
        // If so, same monomer won't be done, since it has been done in
        // previous loop.
        double same = false;
        if (mt1 == mt2) same = true;

        // TODO add neighbour list here
        // Loop over all pair of sites
        for (size_t i = 0; i < ns1; i++) {
          size_t inmon1 = i * nmon1;
          size_t inmon13 = inmon1 * 3;
          for (size_t j = 0; j < ns2; j++) {

            // Check if A = 0 and call the proper field calculation
            double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
            if (A > constants::EPS) {
              A = std::pow(A,1.0/6.0);
              double Ai = 1/A;
              double Asqsq = A*A*A*A;
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                size_t m2init = same ? m1 + 1 : 0;
                elec_field.CalcPermanentElecFieldWithPolfacNonZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        chg.data() + fi_sites1, chg.data() + fi_sites2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, Ai, Asqsq,
                        aCC, aCC1_4, g34, &ex, &ey, &ez, &phi1, 
                        phi.data() + fi_sites2, Efq.data() + fi_crd2);
                phi[fi_sites1 + inmon1 + m1] += phi1;
                Efq[fi_crd1 + inmon13 + m1] += ex;       
                Efq[fi_crd1 + inmon13 + nmon1 + m1] += ey;
                Efq[fi_crd1 + inmon13 + nmon12 + m1] += ez;
              }
            } else {
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                size_t m2init = same ? m1 + 1 : 0;
                elec_field.CalcPermanentElecFieldWithPolfacZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        chg.data() + fi_sites1, chg.data() + fi_sites2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j,
                        &ex, &ey, &ez, &phi1,
                        phi.data() + fi_sites2, Efq.data() + fi_crd2);
                phi[fi_sites1 + inmon1 + m1] += phi1;
                Efq[fi_crd1 + inmon13 + m1] += ex;
                Efq[fi_crd1 + inmon13 + nmon1 + m1] += ey;
                Efq[fi_crd1 + inmon13 + nmon12 + m1] += ez;
              }
            }
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

#   ifdef TIMING
    auto t5 = std::chrono::high_resolution_clock::now();
#   endif

////////////////////////////////////////////////////////////////////////////////
// DIPOLE ELECTRIC FIELD ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Permanent electric field is computed
    // Now start computation of dipole through iteration
    double eps = 1.0E+50;
    std::vector<double> mu_old(3*nsites,0.0);
    size_t iter = 0;

#   ifdef TIMING
    std::vector<std::chrono::milliseconds> dipole_iteration_time_intra;
    std::vector<std::chrono::milliseconds> dipole_iteration_time_inter;
    std::vector<std::chrono::milliseconds> dipole_calculation;
#   endif

    while (true) {   

#     ifdef TIMING
      auto t6 = std::chrono::high_resolution_clock::now();
#     endif

      // TODO Need to decide how we define eps
      double max_eps = 0.0;
      //  Get new dipoles and check max difference
      fi_mon = 0;
      fi_crd = 0;
      fi_sites = 0;
      double alpha = 0.8;
      for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
        size_t ns = sites[fi_mon];
        size_t nmon = mon_type_count[mt].second;
        size_t nmon2 = nmon*2;
        for (size_t i = 0; i < ns; i++) {
          // TODO assuming pol not site dependant
          double p = pol[fi_sites + i];
          size_t inmon3 = 3*i*nmon;
          for (size_t m = 0; m < nmon; m++) {
            mu_old[fi_crd + inmon3 + m] = 
                         mu[fi_crd + inmon3 + m];
            mu_old[fi_crd + inmon3 + nmon + m] = 
                         mu[fi_crd + inmon3 + nmon + m];
            mu_old[fi_crd + inmon3 + nmon2 + m] = 
                         mu[fi_crd + inmon3 + nmon2 + m];
            mu[fi_crd + inmon3 + m] = alpha * p 
                       * (Efq[fi_crd + inmon3 + m] 
                       +  Efd[fi_crd + inmon3 + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + m];  
            mu[fi_crd + inmon3 + nmon + m] = alpha * p 
                       * (Efq[fi_crd + inmon3 + nmon + m] 
                       +  Efd[fi_crd + inmon3 + nmon + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + nmon + m];
            mu[fi_crd + inmon3 + nmon2 + m] = alpha * p 
                       * (Efq[fi_crd + inmon3 + nmon2 + m] 
                       +  Efd[fi_crd + inmon3 + nmon2 + m]) 
                       + (1 - alpha) * mu_old[fi_crd + inmon3 + nmon2 + m];  
          }
          
          // Check for max epsilon
          for (size_t m = 0; m < nmon; m++) {
            double tmpeps = (mu[fi_crd + inmon3 + m] 
                           - mu_old[fi_crd + inmon3 + m])
                           *(mu[fi_crd + inmon3 + m] 
                           - mu_old[fi_crd + inmon3 + m])
                           +(mu[fi_crd + inmon3 + nmon + m]
                           - mu_old[fi_crd + inmon3 + nmon + m])
                           *(mu[fi_crd + inmon3 + nmon + m] 
                           - mu_old[fi_crd + inmon3 + nmon + m])
                           +(mu[fi_crd + inmon3 + nmon2 + m]
                           - mu_old[fi_crd + inmon3 + nmon2 + m])
                           *(mu[fi_crd + inmon3 + nmon2 + m] 
                           - mu_old[fi_crd + inmon3 + nmon2 + m]);
            if (tmpeps > max_eps) max_eps = tmpeps;
          }
        }  
        fi_mon += nmon;
        fi_sites += nmon*ns;
        fi_crd += nmon*ns*3;
      }   

#     ifdef TIMING
      auto t7 = std::chrono::high_resolution_clock::now();
      dipole_calculation.push_back
        (std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6));
#     endif

      // Check if convergence achieved
      if (max_eps < tolerance) 
        break;
      // Check if epsilon is increasing
      if (max_eps > eps && iter > 10) {
        // Exit with error
        std::cerr << "Dipoles diverged" << std::endl;
        std::exit(EXIT_FAILURE);
      } 
      eps = max_eps;

      // If not, check iter number
      if (iter > maxit) {
        // Exit with error
        std::cerr << "Max number of iterations reached" << std::endl;
        std::exit(EXIT_FAILURE);
      }
      iter++;
      //std::cout << iter << std::endl;
      
      std::fill(Efd.begin(), Efd.end(), 0.0);
      
      // Recalculate Electric field due to dipoles
      // Sites on the same monomer
      fi_mon = 0;
      fi_sites = 0;
      fi_crd = 0;

#     ifdef TIMING
      auto t8 = std::chrono::high_resolution_clock::now();
#     endif

      for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
        size_t ns = sites[fi_mon];
        if (ns == 1) continue;
        size_t nmon = mon_type_count[mt].second;
        size_t nmon2 = 2*nmon;
        // Get excluded pairs for this monomer
        systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);
        for (size_t i = 0; i < ns-1 ; i++) {
          size_t inmon3 = 3*i*nmon;
          for (size_t j = i+1; j < ns; j++) {
            // Set the proper aDD
            bool is12 = systools::IsExcluded(exc12, i, j);
            bool is13 = systools::IsExcluded(exc13, i, j);
            bool is14 = systools::IsExcluded(exc14, i, j);
            aDD = systools::GetAdd(is12, is13, is14, mon_id[fi_mon]);

            double A = polfac[fi_sites + i] * polfac[fi_sites + j];
            if (A > constants::EPS) {
              A = std::pow(A, 1.0/6.0);
              double Asqsq = A*A*A*A;
              for (size_t m = 0; m < nmon; m++) {
                // TODO. Slowest function
                elec_field.CalcDipoleElecFieldWithPolfacNonZero(
                          xyz.data() + fi_crd, xyz.data() + fi_crd, 
                          mu.data() + fi_crd, mu.data() + fi_crd, m, m, m + 1,
                          nmon, nmon, i, j, Asqsq,
                          aDD, Efd.data() + fi_crd, &ex, &ey, &ez);
                Efd[fi_crd + inmon3 + m] += ex;
                Efd[fi_crd + inmon3 + nmon + m] += ey;
                Efd[fi_crd + inmon3 + nmon2 + m] += ez;
              }
            } else {
              for (size_t m = 0; m < nmon; m++) {
                elec_field.CalcDipoleElecFieldWithPolfacZero(
                          xyz.data() + fi_crd, xyz.data() + fi_crd,
                          mu.data() + fi_crd, mu.data() + fi_crd, m, m, m + 1,
                          nmon, nmon, i, j, Efd.data() + fi_crd, &ex, &ey, &ez);
                Efd[fi_crd + inmon3 + m] += ex;
                Efd[fi_crd + inmon3 + nmon + m] += ey;
                Efd[fi_crd + inmon3 + nmon2 + m] += ez;
              }
            }
          }
        }
        // Update first indexes
        fi_mon += nmon;
        fi_sites += nmon * ns;
        fi_crd += nmon * ns * 3;
      }

#     ifdef TIMING
      auto t9 = std::chrono::high_resolution_clock::now();
#     endif

      fi_mon1 = 0;
      fi_sites1 = 0;
      fi_mon2 = 0;
      fi_sites2 = 0;
      fi_crd1 = 0;
      fi_crd2 = 0;
      // aDD intermolecular is always 0.055
      aDD = 0.055;
      for (size_t mt1 = 0; mt1 < mon_type_count.size(); mt1++) {
        size_t ns1 = sites[fi_mon1];
        size_t nmon1 = mon_type_count[mt1].second;
        size_t nmon12 = 2 * nmon1;
        fi_mon2 = fi_mon1;
        fi_sites2 = fi_sites1;
        fi_crd2 = fi_crd1;
        for (size_t mt2 = mt1; mt2 < mon_type_count.size(); mt2++) {
          size_t ns2 = sites[fi_mon2];
          size_t nmon2 = mon_type_count[mt2].second;
          double same = false;
          if (mt1 == mt2) same = true;
          // TODO add neighbour list here
          // Prepare for parallelization
#         ifdef _OPENMP
#           pragma omp parallel // omp_get_num_threads() needs to be inside 
                                // parallel region to get number of threads
            { 
              if (omp_get_thread_num() == 0)
                nthreads = omp_get_num_threads();
            }
#         endif
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
                double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
                if (A > constants::EPS) {
                  A = std::pow(A,1.0/6.0);
                  double Asqsq = A*A*A*A;
                  local_field->CalcDipoleElecFieldWithPolfacNonZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        mu.data() + fi_crd1, mu.data() + fi_crd2, 
                        m1, m2init, nmon2,
                        nmon1, nmon2, i, j, Asqsq,
                        aDD, Efd_2_pool[rank].data(), 
                        &ex_thread, &ey_thread, &ez_thread);
                  Efd_1_pool[rank][inmon13 + m1] += ex_thread;
                  Efd_1_pool[rank][inmon13 + nmon1 + m1] += ey_thread;
                  Efd_1_pool[rank][inmon13 + nmon12 + m1] += ez_thread;
                } else {
                  local_field->CalcDipoleElecFieldWithPolfacZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        mu.data() + fi_crd1, mu.data() + fi_crd2, 
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
              Efd[fi_crd1 + k] += Efd_1_pool[rank][k];
            }
            for (size_t k = 0; k < kend2; k++) {
              Efd[fi_crd2 + k] += Efd_2_pool[rank][k];
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

#     ifdef TIMING
      auto t10 = std::chrono::high_resolution_clock::now();
      dipole_iteration_time_intra.push_back
        (std::chrono::duration_cast<std::chrono::milliseconds>(t9 - t8));
      dipole_iteration_time_inter.push_back
        (std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9));
#     endif

    }

#   ifdef TIMING
    std::chrono::milliseconds dp(0);
    std::chrono::milliseconds intra(0);
    std::chrono::milliseconds inter(0);
    for (size_t i = 0; i < dipole_calculation.size(); i++)
      dp += dipole_calculation[i];
    for (size_t i = 0; i < dipole_iteration_time_intra.size(); i++)
      intra += dipole_iteration_time_intra[i];
    for (size_t i = 0; i < dipole_iteration_time_inter.size(); i++)
      inter += dipole_iteration_time_inter[i];
    double dp_d = (double)dp.count() / dipole_calculation.size();
    double intra_d = (double)intra.count() / dipole_iteration_time_intra.size();
    double inter_d = (double)inter.count() / dipole_iteration_time_inter.size();
#   endif

    // Dipoles are computed. Now we need the electrostatic energy.
    // Permanent electrostatics
    double Eqq = 0.0;
    for (size_t i = 0; i < nsites; i++)
      Eqq += phi[i] * chg[i];
    Eqq *= 0.5;

    // Induced Electrostatic energy (chg-dip, dip-dip, pol)
    double Eind = 0.0;
    for (size_t i = 0; i < 3*nsites; i++) 
      Eind -= mu[i] * Efq[i];
    Eind *= 0.5;

    // If no gradients, nothing else to do
    if (!do_grads) return Eqq + Eind;
  
////////////////////////////////////////////////////////////////////////////////
// GRADIENTS ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#   ifdef TIMING
    auto t11 = std::chrono::high_resolution_clock::now();
#   endif

    // Chg-Chg interactions
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t i = 0; i < ns ; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        for (size_t m = 0; m < nmon; m++) {
          grd[fi_crd + inmon3 + m] -= 
            chg[fi_sites + inmon + m]*Efq[fi_crd + inmon3 + m];
          grd[fi_crd + inmon3 + nmon + m] -= 
            chg[fi_sites + inmon + m]*Efq[fi_crd + inmon3 + nmon + m];
          grd[fi_crd + inmon3 + nmon2 + m] -= 
            chg[fi_sites + inmon + m]*Efq[fi_crd + inmon3 + nmon2 + m];
        }
      }
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    // This will only be used for intramonomer to not get chg-dip grd
    std::vector<double> zeros(nsites,0.0);

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);
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
                              0 : phi.data() + fi_sites; 
          aDD = systools::GetAdd(is12, is13, is14, mon_id[fi_mon]);
          double A = polfac[fi_sites + i] * polfac[fi_sites + j];
          if (A > constants::EPS) {
            A = std::pow(A, 1.0/6.0);
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcElecFieldGradsWithPolfacNonZero(
                        xyz.data() + fi_crd, xyz.data() + fi_crd,
                        zeros.data(), zeros.data(),
                        mu.data() + fi_crd, mu.data() + fi_crd,
                        m, m, m+1, nmon, nmon, i, j, aDD, aCD, Asqsq,
                        &ex, &ey, &ez, &phi1, phi_mod,
                        grd.data() + fi_crd);
              grd[fi_crd + inmon3 + m] += ex;
              grd[fi_crd + inmon3 + nmon + m] += ey;
              grd[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              elec_field.CalcElecFieldGradsWithPolfacZero(
                        xyz.data() + fi_crd, xyz.data() + fi_crd,
                        zeros.data(), zeros.data(),
                        mu.data() + fi_crd, mu.data() + fi_crd,
                        m, m, m+1, nmon, nmon, i, j, 
                        &ex, &ey, &ez, &phi1, phi_mod,
                        grd.data() + fi_crd);
              grd[fi_crd + inmon3 + m] += ex;
              grd[fi_crd + inmon3 + nmon + m] += ey;
              grd[fi_crd + inmon3 + nmon2 + m] += ez;
            }
          }
        }
      }
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    fi_mon1 = 0;
    fi_sites1 = 0;
    fi_mon2 = 0;
    fi_sites2 = 0;
    fi_crd1 = 0;
    fi_crd2 = 0;
    // aDD intermolecular is always 0.055
    aDD = 0.055;
    for (size_t mt1 = 0; mt1 < mon_type_count.size(); mt1++) {
      size_t ns1 = sites[fi_mon1];
      size_t nmon1 = mon_type_count[mt1].second;
      size_t nmon12 = nmon1*2;
      fi_mon2 = fi_mon1;
      fi_sites2 = fi_sites1;
      fi_crd2 = fi_crd1;
      for (size_t mt2 = mt1; mt2 < mon_type_count.size(); mt2++) {
        size_t ns2 = sites[fi_mon2];
        size_t nmon2 = mon_type_count[mt2].second;
        double same = false;
        if (mt1 == mt2) same = true;
        // TODO add neighbour list here
        for (size_t i = 0; i < ns1; i++) {
          size_t inmon1  = i*nmon1;
          size_t inmon13  = 3*inmon1;
          for (size_t j = 0; j < ns2; j++) {
            double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
            if (A > constants::EPS) {
              A = std::pow(A,1.0/6.0);
              double Asqsq = A*A*A*A;
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                size_t m2init = same ? m1 + 1 : 0;
                elec_field.CalcElecFieldGradsWithPolfacNonZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        chg.data() + fi_sites1, chg.data() + fi_sites2,
                        mu.data() + fi_crd1, mu.data() + fi_crd2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, 
                        aDD, aCD, Asqsq,
                        &ex, &ey, &ez, &phi1, phi.data() + fi_sites2,
                        grd.data() + fi_crd2);
                grd[fi_crd1 + inmon13 + m1] += ex;
                grd[fi_crd1 + inmon13 + nmon1 + m1] += ey;
                grd[fi_crd1 + inmon13 + nmon12 + m1] += ez;
                phi[fi_sites1 + inmon1 + m1] += phi1;
              }
            } else {
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                size_t m2init = same ? m1 + 1 : 0;
                elec_field.CalcElecFieldGradsWithPolfacZero(
                        xyz.data() + fi_crd1, xyz.data() + fi_crd2,
                        chg.data() + fi_sites1, chg.data() + fi_sites2,
                        mu.data() + fi_crd1, mu.data() + fi_crd2,
                        m1, m2init, nmon2, nmon1, nmon2, i, j, 
                        &ex, &ey, &ez, &phi1, phi.data() + fi_sites2,
                        grd.data() + fi_crd2);
                grd[fi_crd1 + inmon13 + m1] += ex;
                grd[fi_crd1 + inmon13 + nmon1 + m1] += ey;
                grd[fi_crd1 + inmon13 + nmon12 + m1] += ez;
                phi[fi_sites1 + inmon1 + m1] += phi1;
              }
            }
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

#   ifdef TIMING
    auto t12 = std::chrono::high_resolution_clock::now();
#   endif

////////////////////////////////////////////////////////////////////////////////
// REVERT DATA ORGANIZATION ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Reorganize field and potential to initial order
    std::vector<double> tmp1(nsites3,0.0);
    std::vector<double> tmp2(nsites,0.0);
    std::vector<double> tmp3(nsites3,0.0);
    std::vector<double> tmp4(nsites3,0.0);
    fi_mon = 0;
    fi_crd = 0;
    fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t m = 0; m < nmon; m++) {
        size_t mns = m*ns;
        size_t mns3 = mns*3;
        for (size_t i = 0; i < ns; i++) {
          size_t inmon = i*nmon;
          size_t inmon3 = 3*inmon;
          tmp1[fi_crd + mns3 + 3*i] = Efq[inmon3 + m + fi_crd];
          tmp1[fi_crd + mns3 + 3*i + 1] = Efq[inmon3 + m + fi_crd + nmon];
          tmp1[fi_crd + mns3 + 3*i + 2] = Efq[inmon3 + m + fi_crd + nmon2];

          tmp3[fi_crd + mns3 + 3*i] = Efd[inmon3 + m + fi_crd];
          tmp3[fi_crd + mns3 + 3*i + 1] = Efd[inmon3 + m + fi_crd + nmon];
          tmp3[fi_crd + mns3 + 3*i + 2] = Efd[inmon3 + m + fi_crd + nmon2];

          tmp4[fi_crd + mns3 + 3*i] = mu[inmon3 + m + fi_crd];
          tmp4[fi_crd + mns3 + 3*i + 1] = mu[inmon3 + m + fi_crd + nmon];
          tmp4[fi_crd + mns3 + 3*i + 2] = mu[inmon3 + m + fi_crd + nmon2];

          tmp2[fi_sites + mns + i] = phi[fi_sites + m + inmon];

          orig_grd[fi_crd + mns3 + 3*i] += grd[inmon3 + m + fi_crd];
          orig_grd[fi_crd + mns3 + 3*i + 1] += grd[inmon3 + m + fi_crd + nmon];
          orig_grd[fi_crd + mns3 + 3*i + 2] += grd[inmon3 + m + fi_crd + nmon2];
          #ifdef DEBUG
          std::cerr << "phi[" << fi_sites + mns + i << "] = "
                    << tmp2[fi_sites + mns + i] << std::endl;
          std::cerr << "E[" << fi_sites + mns + i << "] = "
                    << tmp1[fi_crd + mns3 + 3*i] << " "
                    << tmp1[fi_crd + mns3 + 3*i + 1] << " "
                    << tmp1[fi_crd + mns3 + 3*i + 2] << " " << std::endl;

          #endif
        }
      }
      fi_mon += nmon;
      fi_sites += nmon*ns;
      fi_crd += nmon*ns*3;
    }
    Efq = tmp1;
    chg = orig_chg;
    phi = tmp2;
    mu = tmp4;
    Efd = tmp3;

#   ifdef TIMING
    auto t13 = std::chrono::high_resolution_clock::now();
#   endif

////////////////////////////////////////////////////////////////////////////////
// REDISTRIBUTION OF THE GRADIENTS AND GRADIENTS DUE TO SITE-DEPENDENT CHARGES /
////////////////////////////////////////////////////////////////////////////////

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      std::string id = mon_id[fi_mon];

      // Redistribute gradients
      systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, orig_grd);

      // Gradients due to position dependant charges
      systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites,
                               phi, orig_grd, chg_grd);
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

#   ifdef TIMING
    auto t14 = std::chrono::high_resolution_clock::now();
    
    // Outputting timings
    std::cerr << "Electrostatics::setup " << 
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
      << " milliseconds\n";
    std::cerr << "Electrostatics::intra_monomer_permanent_elecField " << 
      std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()
      << " milliseconds\n";
    std::cerr << "Electrostatics::inter_monomer_permanent_elecField " << 
      std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count()
      << " milliseconds\n";
    std::cerr << "Electrostatics::Induced_dipole_calculation "
      << dp_d << " milliseconds / cycle over " 
      << dipole_calculation.size() << " cycles\n";
    std::cerr << "Electrostatics::Induced_dipole_intra_dipole_field "
      << intra_d << " milliseconds / cycle over " 
      << dipole_iteration_time_intra.size() << " cycles\n";
    std::cerr << "Electrostatics::Induced_dipole_inter_dipole_field "
      << inter_d << " milliseconds / cycle over " 
      << dipole_iteration_time_inter.size() << " cycles\n";
    std::cerr << "Electrostatics::gradients "
      << std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count()
      << " milliseconds\n";
    std::cerr << "Electrostatics::data_reorganization "
      << std::chrono::duration_cast<std::chrono::milliseconds>(t13 - t12).count()
      << " milliseconds\n";
    std::cerr << "Electrostatics::virtual_sites_grad_redistribution "
      << std::chrono::duration_cast<std::chrono::milliseconds>(t14 - t13).count()
      << " milliseconds\n";
#   endif

    return Eqq + Eind;
    
  } 

} // namespace elec
