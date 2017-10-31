#include "potential/electrostatics/electrostatics.h"

#define DEBUG

namespace elec {

  double Electrostatics(const std::vector<double> chg,               
    const std::vector<double> chg_grd, 
    const std::vector<double> polfac, 
    const std::vector<double> pol, 
    const std::vector<double> orig_xyz, 
    const std::vector<std::string> mon_id, 
    const std::vector<size_t> sites, 
    const std::vector<size_t> first_ind,
    const std::vector<std::pair<std::string,size_t>> mon_type_count, 
    const double tolerance, const size_t maxit, const bool do_grads, 
    std::vector<double> &grad) {

    // Damping declarations
    const double aCC = 0.4;
    const double aCD = 0.4;
    //const double aCD = 0.4;
    double aDD = 0.055;

    // Constants that will be used later
    const double g34 = std::exp(gammln(3.0/4.0));

    // System properties and sizes
    const size_t nsites = chg.size();
    std::vector<double> sqrtpol(3*nsites, 0.0);

    // Electric fields and potential
    std::vector<double> Efq(3*nsites,0.0);
    std::vector<double> Efd(3*nsites,0.0);
    std::vector<double> phi(nsites,0.0);

    // Dipole vector
    std::vector<double> mu(3*nsites,0.0);
    
    // Squareroot all pols
    for (size_t j = 0; j < nsites; j++) {
      const size_t j3 = 3*j;
      const double sq = std::sqrt(pol[j]);
      for (size_t i = 0; i < 3; i++) {
        sqrtpol[j3 + i] = sq;
      }
    }

    // Organize xyz so we have x1_1 x1_2 ... y1_1 y1_2...
    // where xN_M is read as coordinate x of site N of monomer M
    // for the first monomer type. Then follows the second, and so on.
    std::vector<double> xyz(3*nsites,0.0);
    size_t fi_mon = 0;
    size_t fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t m = 0; m < nmon; m++) {
        size_t mns3 = m*ns*3;
        for (size_t i = 0; i < ns; i++) {
          size_t inmon3 = 3*i*nmon;
          xyz[inmon3 + m + fi_crd] = 
                 orig_xyz[fi_crd + mns3 + 3*i];
          xyz[inmon3 + m + fi_crd + nmon] = 
                 orig_xyz[fi_crd + mns3 + 3*i + 1];
          xyz[inmon3 + m + fi_crd + nmon2] = 
                 orig_xyz[fi_crd + mns3 + 3*i + 2];
        }
      }
      fi_mon += nmon;
      fi_crd += nmon*ns*3;
    }

    // Obtain permanent electric field
    // Sites on the same monomer
    fi_mon = 0;
    fi_crd = 0;
    size_t fi_sites = 0;

    // Excluded sets
    excluded_set_type exc12;
    excluded_set_type exc13;
    excluded_set_type exc14;

    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);
      for (size_t i = 0; i < ns -1; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        size_t i3 = 3*i;
        for (size_t j = i + 1; j < ns; j++) {
          size_t jnmon  = j*nmon;
          size_t jnmon3  = 3*jnmon;
          size_t j3 = 3*j;

          // Continue only if i and j are not bonded
          bool is12 = systools::IsExcluded(exc12, i, j);
          bool is13 = systools::IsExcluded(exc13, i, j);
          bool is14 = systools::IsExcluded(exc14, i, j);
          if (is12 || is13 || is14) continue;
          
          // Get a1a2 and check if is not 0.
          double A = polfac[fi_sites + i] * polfac[fi_sites + j];

          std::vector<double> phii(nmon,0.0);
          std::vector<double> phij(nmon,0.0);
          std::vector<double> Efqix(nmon,0.0);
          std::vector<double> Efqjx(nmon,0.0);
          std::vector<double> Efqiy(nmon,0.0);
          std::vector<double> Efqjy(nmon,0.0);
          std::vector<double> Efqiz(nmon,0.0);
          std::vector<double> Efqjz(nmon,0.0);

          if (A > constants::EPS) {
            A = std::pow(A,1.0/6.0);
            double Ai = 1/A;
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              //  Distances and values that will be reused
              const double rijx = xyz[fi_crd + inmon3 + m] 
                                - xyz[fi_crd + jnmon3 + m];
              const double rijy = xyz[fi_crd + inmon3 + m + nmon] 
                                - xyz[fi_crd + jnmon3 + m + nmon];
              const double rijz = xyz[fi_crd + inmon3 + m + nmon2] 
                                - xyz[fi_crd + jnmon3 + m + nmon2];
              const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
              const double r = std::sqrt(rsq);
              const double ri = 1.0/r;
              const double risq = ri*ri;
              const double rsqsq = rsq * rsq;

              // Some values that will be used in the screening functions
              const double rA4 = rsqsq/Asqsq;
              const double arA4 = aCC*rA4;
              // TODO look at the exponential function intel vec
              const double exp1 = std::exp(-arA4);
              const double gq = gammq(3.0/4.0, arA4);
              //const double gq = 1.0;
              const double a_mrt = std::pow(aCC, 1.0/4.0);
              //const double a4 = aCC * 4.0;

              // Get screening functions
              const double s1r = ri - exp1*ri;
              const double s0r = s1r + a_mrt * Ai * g34 * gq;
              const double s1r3 = s1r * risq;

              // Update the field
              const size_t shift = first_ind[fi_mon + m];
              const size_t spi = shift + i;
              const size_t spj = shift + j;
        
              phii[m] = s0r *chg[spj];
              phij[m] = s0r *chg[spi];
     
              // Update Electric field
              Efqix[m] = s1r3 * chg[spj] * rijx;
              Efqjx[m] = s1r3 * chg[spi] * rijx;
              Efqiy[m] = s1r3 * chg[spj] * rijy;
              Efqjy[m] = s1r3 * chg[spi] * rijy;
              Efqiz[m] = s1r3 * chg[spj] * rijz;
              Efqjz[m] = s1r3 * chg[spi] * rijz;
            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              //  Distances and values that will be reused
              const double rijx = xyz[fi_crd + inmon3 + m]
                                - xyz[fi_crd + jnmon3 + m];
              const double rijy = xyz[fi_crd + inmon3 + m + nmon]
                                - xyz[fi_crd + jnmon3 + m + nmon];
              const double rijz = xyz[fi_crd + inmon3 + m + nmon2]
                                - xyz[fi_crd + jnmon3 + m + nmon2];
              const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
              const double r = std::sqrt(rsq);
              const double ri = 1.0/r;
              const double risq = ri*ri;

              // Update the field
              const size_t shift = first_ind[fi_mon + m];
              const size_t spi = shift + i;
              const size_t spj = shift + j;

              phii[m] = ri * chg[spj];
              phij[m] = ri * chg[spi];

              // Update Electric field
              Efqix[m] = ri * risq * chg[spj] * rijx;
              Efqjx[m] = ri * risq * chg[spi] * rijx;
              Efqiy[m] = ri * risq * chg[spj] * rijy;
              Efqjy[m] = ri * risq * chg[spi] * rijy;
              Efqiz[m] = ri * risq * chg[spj] * rijz;
              Efqjz[m] = ri * risq * chg[spi] * rijz;            
            }
          }
          for (size_t m = 0; m < nmon; m++) {
            const size_t shift = first_ind[fi_mon + m];
            const size_t shift3 = 3*shift;
            phi[i + shift] += phii[m];
            phi[j + shift] += phij[m];
            Efq[shift3 + i3] += Efqix[m];
            Efq[shift3 + j3] -= Efqjx[m];
            Efq[shift3 + i3 + 1] += Efqiy[m];
            Efq[shift3 + j3 + 1] -= Efqjy[m];
            Efq[shift3 + i3 + 2] += Efqiz[m];
            Efq[shift3 + j3 + 2] -= Efqjz[m];
          }
        }
      }
      
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    // Sites corresponding to different monomers
    size_t fi_mon1 = 0;
    size_t fi_sites1 = 0;
    size_t fi_mon2 = 0;
    size_t fi_sites2 = 0;
    size_t fi_crd1 = 0;
    size_t fi_crd2 = 0;
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
        size_t nmon22 = nmon2*2;
        double same = false;
        if (mt1 == mt2) same = true;
        // TODO add neighbour list here
        for (size_t i = 0; i < ns1; i++) {
          size_t inmon1  = i*nmon1;
          size_t inmon13  = 3*inmon1;
          size_t i3 = 3*i;
          for (size_t j = 0; j < ns2; j++) {
            size_t jnmon2  = j*nmon2;
            size_t jnmon23  = 3*jnmon2;
            size_t j3 = 3*j;

            double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
           
            if (A > constants::EPS) {
              A = std::pow(A,1.0/6.0);
              double Ai = 1/A;
              double Asqsq = A*A*A*A;
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                std::vector<double> phii(nmon2,0.0);
                std::vector<double> phij(nmon2,0.0);
                std::vector<double> Efqix(nmon2,0.0);
                std::vector<double> Efqjx(nmon2,0.0);
                std::vector<double> Efqiy(nmon2,0.0);
                std::vector<double> Efqjy(nmon2,0.0);
                std::vector<double> Efqiz(nmon2,0.0);
                std::vector<double> Efqjz(nmon2,0.0);
                size_t m2init = same ? m1 + 1 : 0;
                for (size_t m2 = m2init; m2 < nmon2; m2++) {
                  //  Distances and values that will be reused
                  const double rijx = xyz[fi_crd1 + inmon13 + m1] 
                                    - xyz[fi_crd2 + jnmon23 + m2];
                  const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1] 
                                    - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                  const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12] 
                                    - xyz[fi_crd2 + jnmon23 + m2 + nmon22];
                  const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                  const double r = std::sqrt(rsq);
                  const double ri = 1.0/r;
                  const double risq = ri*ri;
                  const double rsqsq = rsq * rsq;
    
                  // Some values that will be used in the screening functions
                  const double rA4 = rsqsq/Asqsq;
                  const double arA4 = aCC*rA4;
                  // TODO look at the exponential function intel vec
                  const double exp1 = std::exp(-arA4);
                  const double gq = gammq(3.0/4.0, arA4);
                  //const double gq = 1.0;
                  const double a_mrt = std::pow(aCC, 1.0/4.0);
                  //const double a4 = aCC * 4.0;
    
                  // Get screening functions
                  const double s1r = ri - exp1*ri;
                  const double s0r = s1r + a_mrt * Ai * g34 * gq;
                  const double s1r3 = s1r * risq;
    
                  // Update the field
                  const size_t shift1 = first_ind[fi_mon1 + m1];
                  const size_t shift2 = first_ind[fi_mon2 + m2];
                  const size_t spi = shift1 + i;
                  const size_t spj = shift2 + j;
    
                  phii[m2] = s0r *chg[spj];
                  phij[m2] = s0r *chg[spi];
    
                  // Update Electric field
                  Efqix[m2] = s1r3 * chg[spj] * rijx;
                  Efqjx[m2] = s1r3 * chg[spi] * rijx;
                  Efqiy[m2] = s1r3 * chg[spj] * rijy;
                  Efqjy[m2] = s1r3 * chg[spi] * rijy;
                  Efqiz[m2] = s1r3 * chg[spj] * rijz;
                  Efqjz[m2] = s1r3 * chg[spi] * rijz;
                }
                
                for (size_t m2 = m2init; m2 < nmon2; m2++) {
                  const size_t shift1 = first_ind[fi_mon1 + m1];
                  const size_t shift2 = first_ind[fi_mon2 + m2];
                  const size_t shift13 = shift1 * 3;
                  const size_t shift23 = shift2 * 3;
                  phi[i + shift1] += phii[m2];
                  phi[j + shift2] += phij[m2];
                  Efq[shift13 + i3] += Efqix[m2];
                  Efq[shift23 + j3] -= Efqjx[m2];
                  Efq[shift13 + i3 + 1] += Efqiy[m2];
                  Efq[shift23 + j3 + 1] -= Efqjy[m2];
                  Efq[shift13 + i3 + 2] += Efqiz[m2];
                  Efq[shift23 + j3 + 2] -= Efqjz[m2];
                }
              }
            } else {
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                std::vector<double> phii(nmon2,0.0);
                std::vector<double> phij(nmon2,0.0);
                std::vector<double> Efqix(nmon2,0.0);
                std::vector<double> Efqjx(nmon2,0.0);
                std::vector<double> Efqiy(nmon2,0.0);
                std::vector<double> Efqjy(nmon2,0.0);
                std::vector<double> Efqiz(nmon2,0.0);
                std::vector<double> Efqjz(nmon2,0.0);
                size_t m2init = same ? m1 + 1 : 0;
                for (size_t m2 = m2init; m2 < nmon2; m2++) {
                  //  Distances and values that will be reused
                  const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                    - xyz[fi_crd2 + jnmon23 + m2];
                  const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                    - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                  const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                    - xyz[fi_crd2 + jnmon23 + m2 + nmon22];
                  const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                  const double r = std::sqrt(rsq);
                  const double ri = 1.0/r;
                  const double risq = ri*ri;
                  
                  // Update the field
                  const size_t shift1 = first_ind[fi_mon1 + m1];
                  const size_t shift2 = first_ind[fi_mon2 + m2];
                  const size_t spi = shift1 + i;
                  const size_t spj = shift2 + j;

                  phii[m2] = ri *chg[spj];
                  phij[m2] = ri *chg[spi];

                  // Update Electric field
                  Efqix[m2] = ri * risq * chg[spj] * rijx;
                  Efqjx[m2] = ri * risq * chg[spi] * rijx;
                  Efqiy[m2] = ri * risq * chg[spj] * rijy;
                  Efqjy[m2] = ri * risq * chg[spi] * rijy;
                  Efqiz[m2] = ri * risq * chg[spj] * rijz;
                  Efqjz[m2] = ri * risq * chg[spi] * rijz;
                }

                for (size_t m2 = m2init; m2 < nmon2; m2++) {
                  const size_t shift1 = first_ind[fi_mon1 + m1];
                  const size_t shift2 = first_ind[fi_mon2 + m2];
                  const size_t shift13 = 3*shift1;
                  const size_t shift23 = 3*shift2;
                  phi[shift1 + i] += phii[m2];
                  phi[shift2 + j] += phij[m2];
                  Efq[shift13 + i3] += Efqix[m2];
                  Efq[shift23 + j3] -= Efqjx[m2];
                  Efq[shift13 + i3 + 1] += Efqiy[m2];
                  Efq[shift23 + j3 + 1] -= Efqjy[m2];
                  Efq[shift13 + i3 + 2] += Efqiz[m2];
                  Efq[shift23 + j3 + 2] -= Efqjz[m2];
                }
              }
            }
          } 
        }
        // Update first indexes
        fi_mon2 += nmon2;
        fi_sites2 += nmon2 * ns2;   
        fi_crd2 += nmon2 * ns2 * 3;
      }
      fi_mon1 += nmon1;
      fi_sites1 += nmon1 * ns1;
      fi_crd1 += nmon1 * ns1 * 3;
    }

    // Permanent electric field is computed
    // Now start computation of dipole through iteration
    double eps = 1.0E+50;
    std::vector<double> mu_old(3*nsites,0.0);
    size_t iter = 0;
    while (true) {   
      // TODO Need to decide how we define eps
      double max_eps = 0.0;
      //  Get new dipoles and check max difference
      for (size_t i = 0; i < nsites; i++) {
        const size_t i3 = 3*i;
        const double p = pol[i];
        double epstmp = 0.0;
        // Learning rate
        double alpha = 0.8;
        for (size_t j = 0; j < 3; j++) {
          mu_old[i3 + j] = mu[i3 + j];
          mu[i3 + j] = alpha * p * (Efq[i3 + j] + Efd[i3 + j])
                       + (1-alpha) * mu_old[i3 + j];
          epstmp += (mu_old[i3 + j] - mu[i3 + j]) 
                    * (mu_old[i3 + j] - mu[i3 + j]);
        }
        if (epstmp > max_eps) max_eps = epstmp;
      }

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
     
      
      std::fill(Efd.begin(), Efd.end(), 0.0);
      
      // Recalculate Electric field due to dipoles
      // Sites on the same monomer
      fi_mon = 0;
      fi_sites = 0;
      fi_crd = 0;
      for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
        size_t ns = sites[fi_mon];
        size_t nmon = mon_type_count[mt].second;
        size_t nmon2 = nmon*2;
        systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);
        for (size_t i = 0; i < ns ; i++) {
          size_t inmon  = i*nmon;
          size_t inmon3  = 3*inmon;
          size_t i3 = 3*i;
          for (size_t j = 0; j < ns; j++) {
            if (i == j) continue;
            size_t jnmon  = j*nmon;
            size_t jnmon3  = 3*jnmon;
            size_t j3 = 3*j;

            // Set the proper aDD
            bool is12 = systools::IsExcluded(exc12, i, j);
            bool is13 = systools::IsExcluded(exc13, i, j);
            bool is14 = systools::IsExcluded(exc14, i, j);
            aDD = systools::GetAdd(is12, is13, is14, mon_id[fi_mon]);
            // This will contain the 9 components of T that then will be
            // added to the field. Inner vector is the one that is contigious,
            // so should not be a problem.
            std::vector<double> Efdcp0(nmon,0.0);
            std::vector<double> Efdcp1(nmon,0.0);
            std::vector<double> Efdcp2(nmon,0.0);
            std::vector<double> Efdcp3(nmon,0.0);
            std::vector<double> Efdcp4(nmon,0.0);
            std::vector<double> Efdcp5(nmon,0.0);
            std::vector<double> Efdcp6(nmon,0.0);
            std::vector<double> Efdcp7(nmon,0.0);
            std::vector<double> Efdcp8(nmon,0.0);
            // Continue only if i and j are not bonded
            double A = polfac[fi_sites + i] * polfac[fi_sites + j];
            if (A > constants::EPS) {
              A = std::pow(A, 1.0/6.0);
              double Asqsq = A*A*A*A;
              for (size_t m = 0; m < nmon; m++) {
                //  Distances and values that will be reused
                const double rijx = xyz[fi_crd + inmon3 + m] 
                                  - xyz[fi_crd + jnmon3 + m];
                const double rijy = xyz[fi_crd + inmon3 + m + nmon] 
                                  - xyz[fi_crd + jnmon3 + m + nmon];
                const double rijz = xyz[fi_crd + inmon3 + m + nmon2] 
                                  - xyz[fi_crd + jnmon3 + m + nmon2];
                const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                const double r = std::sqrt(rsq);
                const double ri = 1.0/r;
                const double risq = ri*ri;
                const double rsqsq = rsq * rsq;
                // Some values that will be used in the screening functions
                const double rA4 = rsqsq/Asqsq;
                const double arA4 = aDD*rA4;
                // TODO look at the exponential function intel vec
                const double exp1 = std::exp(-arA4);
  
                // Get screening functions
                const double s1r = ri - exp1*ri;
                const double s1r3 = s1r * risq;
                const double s2r5_3 = (3.0*s1r3
                                       -4.0*aDD*rA4*exp1*risq*ri)*risq;
                const double ts2x = s2r5_3 * rijx; 
                const double ts2y = s2r5_3 * rijy; 
                const double ts2z = s2r5_3 * rijz; 
                
                Efdcp0[m] = ts2x * rijx - s1r3; // alpha = x, beta = x
                Efdcp1[m] = ts2x * rijy;        // alpha = x, beta = y
                Efdcp2[m] = ts2x * rijz;        // alpha = x, beta = z
                Efdcp3[m] = ts2y * rijx;        // alpha = y, beta = x
                Efdcp4[m] = ts2y * rijy - s1r3; // alpha = y, beta = y
                Efdcp5[m] = ts2y * rijz;        // alpha = y, beta = z
                Efdcp6[m] = ts2z * rijx;        // alpha = z, beta = x
                Efdcp7[m] = ts2z * rijy;        // alpha = z, beta = y
                Efdcp8[m] = ts2z * rijz - s1r3; // alpha = z, beta = z
              }
            } else {
              for (size_t m = 0; m < nmon; m++) {
                //  Distances and values that will be reused
                const double rijx = xyz[fi_crd + inmon3 + m]
                                  - xyz[fi_crd + jnmon3 + m];
                const double rijy = xyz[fi_crd + inmon3 + m + nmon]
                                  - xyz[fi_crd + jnmon3 + m + nmon];
                const double rijz = xyz[fi_crd + inmon3 + m + nmon2]
                                  - xyz[fi_crd + jnmon3 + m + nmon2];
                const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                const double r = std::sqrt(rsq);
                const double ri = 1.0/r;
                const double risq = ri*ri;

                // Get screening functions
                const double s1r3 = ri * risq;
                const double s2r5_3 = (3.0*s1r3) * risq;
                const double ts2x = s2r5_3 * rijx;
                const double ts2y = s2r5_3 * rijy;
                const double ts2z = s2r5_3 * rijz;

                Efdcp0[m] = ts2x * rijx - s1r3; // alpha = x, beta = x
                Efdcp1[m] = ts2x * rijy;        // alpha = x, beta = y
                Efdcp2[m] = ts2x * rijz;        // alpha = x, beta = z
                Efdcp3[m] = ts2y * rijx;        // alpha = y, beta = x
                Efdcp4[m] = ts2y * rijy - s1r3; // alpha = y, beta = y
                Efdcp5[m] = ts2y * rijz;        // alpha = y, beta = z
                Efdcp6[m] = ts2z * rijx;        // alpha = z, beta = x
                Efdcp7[m] = ts2z * rijy;        // alpha = z, beta = y
                Efdcp8[m] = ts2z * rijz - s1r3; // alpha = z, beta = z
              }
            }

            for (size_t m = 0; m < nmon; m++) {
              size_t shift = fi_crd + 3*ns*m;
              Efd[shift + i3] += Efdcp0[m] * mu[shift + j3]
                                       +  Efdcp1[m] * mu[shift + j3 + 1]
                                       +  Efdcp2[m] * mu[shift + j3 + 2];
              Efd[shift + i3 + 1] += Efdcp3[m] * mu[shift + j3]
                                           +  Efdcp4[m] * mu[shift + j3 + 1]
                                           +  Efdcp5[m] * mu[shift + j3 + 2];
              Efd[shift + i3 + 2] += Efdcp6[m] * mu[shift + j3]
                                           +  Efdcp7[m] * mu[shift + j3 + 1]
                                           +  Efdcp8[m] * mu[shift + j3 + 2];
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
          size_t nmon22 = nmon2*2;
          double same = false;
          if (mt1 == mt2) same = true;
          // TODO add neighbour list here
          for (size_t i = 0; i < ns1; i++) {
            size_t inmon1  = i*nmon1;
            size_t inmon13  = 3*inmon1;
            size_t i3 = 3*i;
            for (size_t j = 0; j < ns2; j++) {
              size_t jnmon2  = j*nmon2;
              size_t jnmon23  = 3*jnmon2;
              size_t j3 = 3*j;

              double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
              if (A > constants::EPS) {
                A = std::pow(A,1.0/6.0);
                double Asqsq = A*A*A*A;
                for (size_t m1 = 0; m1 < nmon1; m1++) {
                  // This will contain the 9 components of T that then will be
                  // added to the field. Inner vector is the one that is contigious,
                  // so should not be a problem.
                  std::vector<double> Efdcp0(nmon2,0.0);
                  std::vector<double> Efdcp1(nmon2,0.0);
                  std::vector<double> Efdcp2(nmon2,0.0);
                  std::vector<double> Efdcp3(nmon2,0.0);
                  std::vector<double> Efdcp4(nmon2,0.0);
                  std::vector<double> Efdcp5(nmon2,0.0);
                  std::vector<double> Efdcp6(nmon2,0.0);
                  std::vector<double> Efdcp7(nmon2,0.0);
                  std::vector<double> Efdcp8(nmon2,0.0);
                  if (same) {
                    for (size_t m2 = 0; m2 < m1; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;
                      const double rsqsq = rsq * rsq;
                      // Some values that will be used in the screening functions
                      const double rA4 = rsqsq/Asqsq;
                      const double arA4 = aDD*rA4;
                      // TODO look at the exponential function intel vec
                      const double exp1 = std::exp(-arA4);
    
                      // Get screening functions
                      const double s1r = ri - exp1*ri;
                      const double s1r3 = s1r * risq;
                      const double s2r5_3 = (3.0*s1r3
                                            -4.0*aDD*rA4*exp1*risq*ri)*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;

                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                    for (size_t m2 = m1 + 1; m2 < nmon2; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;
                      const double rsqsq = rsq * rsq;
                      // Some values that will be used in the screening functions
                      const double rA4 = rsqsq/Asqsq;
                      const double arA4 = aDD*rA4;
                      // TODO look at the exponential function intel vec
                      const double exp1 = std::exp(-arA4);
    
                      // Get screening functions
                      const double s1r = ri - exp1*ri;
                      const double s1r3 = s1r * risq;
                      const double s2r5_3 = (3.0*s1r3
                                            -4.0*aDD*rA4*exp1*risq*ri)*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;
    
                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                  } else {
                    for (size_t m2 = 0; m2 < nmon2; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;
                      const double rsqsq = rsq * rsq;
                      // Some values that will be used in the screening functions
                      const double rA4 = rsqsq/Asqsq;
                      const double arA4 = aDD*rA4;
                      // TODO look at the exponential function intel vec
                      const double exp1 = std::exp(-arA4);
    
                      // Get screening functions
                      const double s1r = ri - exp1*ri;
                      const double s1r3 = s1r * risq;
                      const double s2r5_3 = (3.0*s1r3
                                            -4.0*aDD*rA4*exp1*risq*ri)*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;
    
                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                  }

                  size_t shifti = fi_crd1 + 3*ns1*m1 + i3;
                  for (size_t m2 = 0; m2 < nmon2; m2++) {
                    size_t shiftj = fi_crd2 + 3*ns2*m2 + j3;
                    Efd[shifti] += Efdcp0[m2] * mu[shiftj]
                                +  Efdcp1[m2] * mu[shiftj + 1]
                                +  Efdcp2[m2] * mu[shiftj + 2];
                    Efd[shifti + 1] += Efdcp3[m2] * mu[shiftj]
                                    +  Efdcp4[m2] * mu[shiftj + 1]
                                    +  Efdcp5[m2] * mu[shiftj + 2];
                    Efd[shifti + 2] += Efdcp6[m2] * mu[shiftj]
                                    +  Efdcp7[m2] * mu[shiftj + 1]
                                    +  Efdcp8[m2] * mu[shiftj + 2];
                  }
                }
              } else {
                for (size_t m1 = 0; m1 < nmon1; m1++) {
                  // This will contain the 9 components of T that then will be
                  // added to the field. Inner vector is the one that is contigious,
                  // so should not be a problem.
                  std::vector<double> Efdcp0(nmon2,0.0);
                  std::vector<double> Efdcp1(nmon2,0.0);
                  std::vector<double> Efdcp2(nmon2,0.0);
                  std::vector<double> Efdcp3(nmon2,0.0);
                  std::vector<double> Efdcp4(nmon2,0.0);
                  std::vector<double> Efdcp5(nmon2,0.0);
                  std::vector<double> Efdcp6(nmon2,0.0);
                  std::vector<double> Efdcp7(nmon2,0.0);
                  std::vector<double> Efdcp8(nmon2,0.0);
                  if (same) {
                    for (size_t m2 = 0; m2 < m1; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;

                      // Get screening functions
                      const double s1r3 = ri * risq;
                      const double s2r5_3 = 3.0*s1r3*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;

                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                    for (size_t m2 = m1 + 1; m2 < nmon2; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;

                      // Get screening functions
                      const double s1r3 = ri * risq;
                      const double s2r5_3 = 3.0*s1r3*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;

                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                  } else {
                    for (size_t m2 = 0; m2 < nmon2; m2++) {
                      //  Distances and values that will be reused
                      const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                        - xyz[fi_crd2 + jnmon23 + m2];
                      const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                      const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                        - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                      const double rsq = rijx*rijx + rijy*rijy + rijz*rijz;
                      const double r = std::sqrt(rsq);
                      const double ri = 1.0/r;
                      const double risq = ri*ri;

                      // Get screening functions
                      const double s1r3 = ri * risq;
                      const double s2r5_3 = 3.0*s1r3*risq;
                      const double ts2x = s2r5_3 * rijx;
                      const double ts2y = s2r5_3 * rijy;
                      const double ts2z = s2r5_3 * rijz;

                      Efdcp0[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                      Efdcp1[m2] = ts2x * rijy;        // alpha = x, beta = y
                      Efdcp2[m2] = ts2x * rijz;        // alpha = x, beta = z
                      Efdcp3[m2] = ts2y * rijx;        // alpha = y, beta = x
                      Efdcp4[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                      Efdcp5[m2] = ts2y * rijz;        // alpha = y, beta = z
                      Efdcp6[m2] = ts2z * rijx;        // alpha = z, beta = x
                      Efdcp7[m2] = ts2z * rijy;        // alpha = z, beta = y
                      Efdcp8[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                    }
                  }
                  size_t shifti = fi_crd1 + 3*ns1*m1 + i3;
                  for (size_t m2 = 0; m2 < nmon2; m2++) {
                    size_t shiftj = fi_crd2 + 3*ns2*m2 + j3;
                    Efd[shifti] += Efdcp0[m2] * mu[shiftj]
                                +  Efdcp1[m2] * mu[shiftj + 1]
                                +  Efdcp2[m2] * mu[shiftj + 2];
                    Efd[shifti + 1] += Efdcp3[m2] * mu[shiftj]
                                    +  Efdcp4[m2] * mu[shiftj + 1]
                                    +  Efdcp5[m2] * mu[shiftj + 2];
                    Efd[shifti + 2] += Efdcp6[m2] * mu[shiftj]
                                    +  Efdcp7[m2] * mu[shiftj + 1]
                                    +  Efdcp8[m2] * mu[shiftj + 2];
                  }
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
    }

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
  
    // Chg-Chg interactions
    for (size_t i = 0; i < nsites; i++)
      for (size_t j = 0; j < 3; j++) 
        grad[3*i + j] -= chg[i]*Efq[3*i + j];

    // Intramonomer dipole-dipole
    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      systools::GetExcluded(mon_id[fi_mon], exc12, exc13, exc14);
      for (size_t i = 0; i < ns ; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        size_t i3 = 3*i;
        for (size_t j = 0; j < ns; j++) {
          if (i == j) continue;
          size_t jnmon  = j*nmon;
          size_t jnmon3  = 3*jnmon;
          size_t j3 = 3*j;

          // Set the proper aDD
          bool is12 = systools::IsExcluded(exc12, i, j);
          bool is13 = systools::IsExcluded(exc13, i, j);
          bool is14 = systools::IsExcluded(exc14, i, j);
          aDD = systools::GetAdd(is12, is13, is14, mon_id[fi_mon]);
          // This will contain the 10 unique components of T that then will be
          // added to the gradients. 
          std::vector<double> Efdcp0(nmon,0.0);
          std::vector<double> Efdcp1(nmon,0.0);
          std::vector<double> Efdcp2(nmon,0.0);
          std::vector<double> Efdcp3(nmon,0.0);
          std::vector<double> Efdcp4(nmon,0.0);
          std::vector<double> Efdcp5(nmon,0.0);
          std::vector<double> Efdcp6(nmon,0.0);
          std::vector<double> Efdcp7(nmon,0.0);
          std::vector<double> Efdcp8(nmon,0.0);
          std::vector<double> Efdcp9(nmon,0.0);
          double A = polfac[fi_sites + i] * polfac[fi_sites + j];
          if (A > constants::EPS) {
            const double aDD4 = 4.0 * aDD;
            A = std::pow(A, 1.0/6.0);
            double Asqsq = A*A*A*A;
            for (size_t m = 0; m < nmon; m++) {
              //  Distances and values that will be reused
              const double rijx = xyz[fi_crd + inmon3 + m]
                                - xyz[fi_crd + jnmon3 + m];
              const double rijy = xyz[fi_crd + inmon3 + m + nmon]
                                - xyz[fi_crd + jnmon3 + m + nmon];
              const double rijz = xyz[fi_crd + inmon3 + m + nmon2]
                                - xyz[fi_crd + jnmon3 + m + nmon2];
              const double rijx2 = rijx*rijx;
              const double rijy2 = rijy*rijy;
              const double rijz2 = rijz*rijz;
              const double rsq = rijx2 + rijy2 + rijz2;
              const double r = std::sqrt(rsq);
              const double ri = 1.0/r;
              const double risq = ri*ri;
              const double rsqsq = rsq * rsq;
              // Some values that will be used in the screening functions
              const double rA4 = rsqsq/Asqsq;
              const double arA4 = aDD4*rA4;
              // TODO look at the exponential function intel vec
              const double exp1 = std::exp(-aDD*rA4);

              // Get screening functions
              const double s1r = ri - exp1*ri;
              const double s1r3 = s1r * risq;
              const double s2r5_3 = (3.0*s1r3
                                     -arA4*exp1*risq*ri)*risq;
              
              const double s3r7_15 = (s2r5_3 * 5.0 - arA4 * exp1 * 
                      (arA4 - 1.0) * risq * risq * ri) * risq;
              const double s3r7_15x2 = s3r7_15 * rijx2;
              const double s3r7_15y2 = s3r7_15 * rijy2;
              const double s3r7_15z2 = s3r7_15 * rijz2;

              Efdcp0[m] = s3r7_15x2* rijx 
                        - s2r5_3 * 3.0 * rijx;          // x x x

              Efdcp1[m] = s3r7_15x2 * rijy 
                        - s2r5_3 * rijy;                // x x y
              Efdcp2[m] = s3r7_15x2 * rijz 
                        - s2r5_3 * rijz;                // x x z
              Efdcp3[m] = s3r7_15y2 * rijx 
                        - s2r5_3 * rijx;                // x y y
              Efdcp4[m] = s3r7_15 * rijx * rijy * rijz; // x y z
              Efdcp5[m] = s3r7_15z2 * rijx 
                        - s2r5_3 * rijx;                // x z z
              Efdcp6[m] = s3r7_15 * rijy2 * rijy 
                        - s2r5_3 * 3.0 * rijy;          // y y y
              Efdcp7[m] = s3r7_15y2 * rijz 
                        - s2r5_3 * rijz;                // y y z
              Efdcp8[m] = s3r7_15z2 * rijy 
                        - s2r5_3 * rijy;                // y z z
              Efdcp9[m] = s3r7_15z2 * rijz 
                        - s2r5_3 * 3.0 * rijz;          // z z z

            }
          } else {
            for (size_t m = 0; m < nmon; m++) {
              //  Distances and values that will be reused
              const double rijx = xyz[fi_crd + inmon3 + m]
                                - xyz[fi_crd + jnmon3 + m];
              const double rijy = xyz[fi_crd + inmon3 + m + nmon]
                                - xyz[fi_crd + jnmon3 + m + nmon];
              const double rijz = xyz[fi_crd + inmon3 + m + nmon2]
                                - xyz[fi_crd + jnmon3 + m + nmon2];
              const double rijx2 = rijx*rijx;
              const double rijy2 = rijy*rijy;
              const double rijz2 = rijz*rijz;
              const double rsq = rijx2 + rijy2 + rijz2;
              const double r = std::sqrt(rsq);
              const double ri = 1.0/r;
              const double risq = ri*ri;
              // Some values that will be used in the screening functions

              // Get screening functions
              const double s1r3 = ri * risq;
              const double s2r5_3 = 3.0*s1r3*risq;
              const double s3r7_15 = s2r5_3 * 5.0 * risq;

              const double s3r7_15x2 = s3r7_15 * rijx2;
              const double s3r7_15y2 = s3r7_15 * rijy2;
              const double s3r7_15z2 = s3r7_15 * rijz2;

              Efdcp0[m] = s3r7_15x2* rijx
                        - s2r5_3 * 3.0 * rijx;          // x x x

              Efdcp1[m] = s3r7_15x2 * rijy
                        - s2r5_3 * rijy;                // x x y
              Efdcp2[m] = s3r7_15x2 * rijz
                        - s2r5_3 * rijz;                // x x z
              Efdcp3[m] = s3r7_15y2 * rijx
                        - s2r5_3 * rijx;                // x y y
              Efdcp4[m] = s3r7_15 * rijx * rijy * rijz; // x y z
              Efdcp5[m] = s3r7_15z2 * rijx
                        - s2r5_3 * rijx;                // x z z
              Efdcp6[m] = s3r7_15 * rijy2 * rijy
                        - s2r5_3 * 3.0 * rijy;          // y y y
              Efdcp7[m] = s3r7_15y2 * rijz
                        - s2r5_3 * rijz;                // y y z
              Efdcp8[m] = s3r7_15z2 * rijy
                        - s2r5_3 * rijy;                // y z z
              Efdcp9[m] = s3r7_15z2 * rijz
                        - s2r5_3 * 3.0 * rijz;          // z z z
            }
          }

          for (size_t m = 0; m < nmon; m++) {
            size_t shift = fi_crd + 3*ns*m;
            grad[shift + i3] += 
                 (Efdcp0[m]*mu[shift + i3]*mu[shift + j3]           // x x x
                + Efdcp1[m]*mu[shift + i3]*mu[shift + j3 + 1]       // x x y
                + Efdcp2[m]*mu[shift + i3]*mu[shift + j3 + 2]       // x x z
                + Efdcp1[m]*mu[shift + i3 + 1]*mu[shift + j3]       // x y x
                + Efdcp3[m]*mu[shift + i3 + 1]*mu[shift + j3 + 1]   // x y y
                + Efdcp4[m]*mu[shift + i3 + 1]*mu[shift + j3 + 2]   // x y z
                + Efdcp2[m]*mu[shift + i3 + 2]*mu[shift + j3]       // x z x
                + Efdcp4[m]*mu[shift + i3 + 2]*mu[shift + j3 + 1]   // x z y
                + Efdcp5[m]*mu[shift + i3 + 2]*mu[shift + j3 + 2]); // x z z
            grad[shift + i3 + 1] += 
                 (Efdcp1[m]*mu[shift + i3]*mu[shift + j3]           // y x x
                + Efdcp3[m]*mu[shift + i3]*mu[shift + j3 + 1]       // y x y
                + Efdcp4[m]*mu[shift + i3]*mu[shift + j3 + 2]       // y x z
                + Efdcp3[m]*mu[shift + i3 + 1]*mu[shift + j3]       // y y x
                + Efdcp6[m]*mu[shift + i3 + 1]*mu[shift + j3 + 1]   // y y y
                + Efdcp7[m]*mu[shift + i3 + 1]*mu[shift + j3 + 2]   // y y z
                + Efdcp4[m]*mu[shift + i3 + 2]*mu[shift + j3]       // y z x
                + Efdcp7[m]*mu[shift + i3 + 2]*mu[shift + j3 + 1]   // y z y
                + Efdcp8[m]*mu[shift + i3 + 2]*mu[shift + j3 + 2]); // y z z
            grad[shift + i3 + 2] += 
                 (Efdcp2[m]*mu[shift + i3]*mu[shift + j3]           // z x x
                + Efdcp4[m]*mu[shift + i3]*mu[shift + j3 + 1]       // z x y
                + Efdcp5[m]*mu[shift + i3]*mu[shift + j3 + 2]       // z x z
                + Efdcp4[m]*mu[shift + i3 + 1]*mu[shift + j3]       // z y x
                + Efdcp7[m]*mu[shift + i3 + 1]*mu[shift + j3 + 1]   // z y y
                + Efdcp8[m]*mu[shift + i3 + 1]*mu[shift + j3 + 2]   // z y z
                + Efdcp5[m]*mu[shift + i3 + 2]*mu[shift + j3]       // z z x
                + Efdcp8[m]*mu[shift + i3 + 2]*mu[shift + j3 + 1]   // z z y
                + Efdcp9[m]*mu[shift + i3 + 2]*mu[shift + j3 + 2]); // z z z
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
    const double aDD4 = 4 * aDD;
    const double aCD4 = 4 * aCD;
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
        size_t nmon22 = nmon2*2;
        double same = false;
        if (mt1 == mt2) same = true;
        // TODO add neighbour list here
        for (size_t i = 0; i < ns1; i++) {
          size_t inmon1  = i*nmon1;
          size_t inmon13  = 3*inmon1;
          size_t i3 = 3*i;
          for (size_t j = 0; j < ns2; j++) {
            size_t jnmon2  = j*nmon2;
            size_t jnmon23  = 3*jnmon2;
            size_t j3 = 3*j;

            double A = polfac[fi_sites1 + i] * polfac[fi_sites2 + j];
            if (A > constants::EPS) {
              A = std::pow(A,1.0/6.0);
              double Asqsq = A*A*A*A;
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                // This will contain the 10 unique components of T that then will be
                // added to the gradients. 
                std::vector<double> Efdcp0(nmon2,0.0);
                std::vector<double> Efdcp1(nmon2,0.0);
                std::vector<double> Efdcp2(nmon2,0.0);
                std::vector<double> Efdcp3(nmon2,0.0);
                std::vector<double> Efdcp4(nmon2,0.0);
                std::vector<double> Efdcp5(nmon2,0.0);
                std::vector<double> Efdcp6(nmon2,0.0);
                std::vector<double> Efdcp7(nmon2,0.0);
                std::vector<double> Efdcp8(nmon2,0.0);
                std::vector<double> Efdcp9(nmon2,0.0);
                
                // This will contain the 6 unique components of T that then will be
                // added to the gradients.
                std::vector<double> Efdtmpcp10(nmon2,0.0);
                std::vector<double> Efdtmpcp11(nmon2,0.0);
                std::vector<double> Efdtmpcp12(nmon2,0.0);
                std::vector<double> Efdtmpcp13(nmon2,0.0);
                std::vector<double> Efdtmpcp14(nmon2,0.0);
                std::vector<double> Efdtmpcp15(nmon2,0.0);

                std::vector<double> rx(nmon2,0.0);
                std::vector<double> ry(nmon2,0.0);
                std::vector<double> rz(nmon2,0.0);
                if (same) {
                  for (size_t m2 = 0; m2 < m1; m2++) {
                    const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                      - xyz[fi_crd2 + jnmon23 + m2];
                    const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    const double rsqsq = rsq * rsq;
                    // Some values that will be used in the screening functions
                    const double rA4 = rsqsq/Asqsq;
                    // TODO look at the exponential function intel vec

                    const double adrA4 = aDD4*rA4;
                    const double acrA4 = aCD4*rA4;
                    const double exp1d = std::exp(-aDD*rA4);
                    const double exp1c = std::exp(-aCD*rA4);

                    // Get screening functions
                    const double s1rd = ri - exp1d*ri;
                    const double s1r3d = s1rd * risq;
                    const double s2r5_3d = (3.0*s1r3d
                                           -adrA4*exp1d*risq*ri)*risq;

                    const double s3r7_15d = (s2r5_3d * 5.0 - adrA4 * exp1d *
                            (adrA4 - 1.0) * risq * risq * ri) * risq;
                    const double s3r7_15x2 = s3r7_15d * rijx2;
                    const double s3r7_15y2 = s3r7_15d * rijy2;
                    const double s3r7_15z2 = s3r7_15d * rijz2;

                    const double s1rc = ri - exp1c*ri;
                    const double s1r3c = s1rc * risq;
                    rx[m2] = rijx * s1r3c;
                    ry[m2] = rijy * s1r3c;
                    rz[m2] = rijz * s1r3c;
                    const double s2r5_3c = (3.0*s1r3c
                                           -acrA4*exp1c*risq*ri)*risq;
                    const double ts2x = s2r5_3c * rijx;
                    const double ts2y = s2r5_3c * rijy;
                    const double ts2z = s2r5_3c * rijz;

                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3d * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3d * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3d * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3d * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15d * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3d * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15d * rijy2 * rijy
                              - s2r5_3d * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3d * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3d * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3d * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3c; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3c; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3c; // alpha = z, beta = z
                  }
                  for (size_t m2 = m1 + 1; m2 < nmon2; m2++) {
                    const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                      - xyz[fi_crd2 + jnmon23 + m2];
                    const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    const double rsqsq = rsq * rsq;
                    // Some values that will be used in the screening functions
                    const double rA4 = rsqsq/Asqsq;
                    // TODO look at the exponential function intel vec

                    const double adrA4 = aDD4*rA4;
                    const double acrA4 = aCD4*rA4;
                    const double exp1d = std::exp(-aDD*rA4);
                    const double exp1c = std::exp(-aCD*rA4);

                    // Get screening functions
                    const double s1rd = ri - exp1d*ri;
                    const double s1r3d = s1rd * risq;
                    const double s2r5_3d = (3.0*s1r3d
                                           -adrA4*exp1d*risq*ri)*risq;

                    const double s3r7_15d = (s2r5_3d * 5.0 - adrA4 * exp1d *
                            (adrA4 - 1.0) * risq * risq * ri) * risq;
                    const double s3r7_15x2 = s3r7_15d * rijx2;
                    const double s3r7_15y2 = s3r7_15d * rijy2;
                    const double s3r7_15z2 = s3r7_15d * rijz2;
                    
                    const double s1rc = ri - exp1c*ri;
                    const double s1r3c = s1rc * risq;
                    rx[m2] = rijx * s1r3c;
                    ry[m2] = rijy * s1r3c;
                    rz[m2] = rijz * s1r3c;
                    const double s2r5_3c = (3.0*s1r3c
                                           -acrA4*exp1c*risq*ri)*risq;
                    const double ts2x = s2r5_3c * rijx;
                    const double ts2y = s2r5_3c * rijy;
                    const double ts2z = s2r5_3c * rijz;

                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3d * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3d * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3d * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3d * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15d * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3d * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15d * rijy2 * rijy
                              - s2r5_3d * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3d * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3d * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3d * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3c; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3c; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3c; // alpha = z, beta = z
                  }
                } else {
                  for (size_t m2 = 0; m2 < nmon2; m2++) {
                    const double rijx = xyz[fi_crd + inmon13 + m1]
                                      - xyz[fi_crd + jnmon23 + m2];
                    const double rijy = xyz[fi_crd + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd + jnmon23 + m2 + nmon22];

                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    const double rsqsq = rsq * rsq;
                    // Some values that will be used in the screening functions
                    const double rA4 = rsqsq/Asqsq;
                    // TODO look at the exponential function intel vec

                    const double adrA4 = aDD4*rA4;
                    const double acrA4 = aCD4*rA4;
                    const double exp1d = std::exp(-aDD*rA4);
                    const double exp1c = std::exp(-aCD*rA4);

                    // Get screening functions
                    const double s1rd = ri - exp1d*ri;
                    const double s1r3d = s1rd * risq;
                    const double s2r5_3d = (3.0*s1r3d
                                           -adrA4*exp1d*risq*ri)*risq;

                    const double s3r7_15d = (s2r5_3d * 5.0 - adrA4 * exp1d *
                            (adrA4 - 1.0) * risq * risq * ri) * risq;
                    const double s3r7_15x2 = s3r7_15d * rijx2;
                    const double s3r7_15y2 = s3r7_15d * rijy2;
                    const double s3r7_15z2 = s3r7_15d * rijz2;

                    const double s1rc = ri - exp1c*ri;
                    const double s1r3c = s1rc * risq;
                    rx[m2] = rijx * s1r3c;
                    ry[m2] = rijy * s1r3c;
                    rz[m2] = rijz * s1r3c;
                    const double s2r5_3c = (3.0*s1r3c
                                           -acrA4*exp1c*risq*ri)*risq;
                    const double ts2x = s2r5_3c * rijx;
                    const double ts2y = s2r5_3c * rijy;
                    const double ts2z = s2r5_3c * rijz;
 
                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3d * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3d * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3d * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3d * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15d * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3d * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15d * rijy2 * rijy
                              - s2r5_3d * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3d * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3d * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3d * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3c; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3c; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3c; // alpha = z, beta = z
                  }
                }
                size_t shifti = fi_crd1 + 3*ns1*m1 + i3;
                size_t sii = fi_sites1 + ns1*m1 + i;
                for (size_t m2 = 0; m2 < nmon2; m2++) {
                  size_t shiftj = fi_crd2 + 3*ns2*m2 + j3;
                  size_t sij = fi_sites2 + ns2*m2 + j;
                  // chg-dip grad
                  grad[shifti] -= 
                     (chg[sii] * Efdtmpcp10[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp10[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp11[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp11[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp12[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp12[m2] * mu[shifti + 2]);
                  grad[shifti + 1] -=
                     (chg[sii] * Efdtmpcp11[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp11[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp13[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp13[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp14[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp14[m2] * mu[shifti + 2]);
                  grad[shifti + 2] -=
                     (chg[sii] * Efdtmpcp12[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp12[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp14[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp14[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp15[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp15[m2] * mu[shifti + 2]);

                  // Update potential
                  // Sign due to distance
                  phi[sij] -= rx[m2]*mu[shifti] + ry[m2]*mu[shifti + 1]
                            + rz[m2]*mu[shifti + 2];

                  // dip-dip
                  grad[shifti] +=
                    (Efdcp0[m2]*mu[shifti]*mu[shiftj]           // x x x
                   + Efdcp1[m2]*mu[shifti]*mu[shiftj + 1]       // x x y
                   + Efdcp2[m2]*mu[shifti]*mu[shiftj + 2]       // x x z
                   + Efdcp1[m2]*mu[shifti + 1]*mu[shiftj]       // x y x
                   + Efdcp3[m2]*mu[shifti + 1]*mu[shiftj + 1]   // x y y
                   + Efdcp4[m2]*mu[shifti + 1]*mu[shiftj + 2]   // x y z
                   + Efdcp2[m2]*mu[shifti + 2]*mu[shiftj]       // x z x
                   + Efdcp4[m2]*mu[shifti + 2]*mu[shiftj + 1]   // x z y
                   + Efdcp5[m2]*mu[shifti + 2]*mu[shiftj + 2]); // x z z
                  grad[shifti + 1] +=
                    (Efdcp1[m2]*mu[shifti]*mu[shiftj]           // y x x
                   + Efdcp3[m2]*mu[shifti]*mu[shiftj + 1]       // y x y
                   + Efdcp4[m2]*mu[shifti]*mu[shiftj + 2]       // y x z
                   + Efdcp3[m2]*mu[shifti + 1]*mu[shiftj]       // y y x
                   + Efdcp6[m2]*mu[shifti + 1]*mu[shiftj + 1]   // y y y
                   + Efdcp7[m2]*mu[shifti + 1]*mu[shiftj + 2]   // y y z
                   + Efdcp4[m2]*mu[shifti + 2]*mu[shiftj]       // y z x
                   + Efdcp7[m2]*mu[shifti + 2]*mu[shiftj + 1]   // y z y
                   + Efdcp8[m2]*mu[shifti + 2]*mu[shiftj + 2]); // y z z
                  grad[shifti + 2] +=
                    (Efdcp2[m2]*mu[shifti]*mu[shiftj]           // z x x
                   + Efdcp4[m2]*mu[shifti]*mu[shiftj + 1]       // z x y
                   + Efdcp5[m2]*mu[shifti]*mu[shiftj + 2]       // z x z
                   + Efdcp4[m2]*mu[shifti + 1]*mu[shiftj]       // z y x
                   + Efdcp7[m2]*mu[shifti + 1]*mu[shiftj + 1]   // z y y
                   + Efdcp8[m2]*mu[shifti + 1]*mu[shiftj + 2]   // z y z
                   + Efdcp5[m2]*mu[shifti + 2]*mu[shiftj]       // z z x
                   + Efdcp8[m2]*mu[shifti + 2]*mu[shiftj + 1]   // z z y
                   + Efdcp9[m2]*mu[shifti + 2]*mu[shiftj + 2]); // z z z
                }
              }
            } else {
              for (size_t m1 = 0; m1 < nmon1; m1++) {
                // This will contain the 10 unique components of T that then will be
                // added to the gradients. 
                std::vector<double> Efdcp0(nmon2,0.0);
                std::vector<double> Efdcp1(nmon2,0.0);
                std::vector<double> Efdcp2(nmon2,0.0);
                std::vector<double> Efdcp3(nmon2,0.0);
                std::vector<double> Efdcp4(nmon2,0.0);
                std::vector<double> Efdcp5(nmon2,0.0);
                std::vector<double> Efdcp6(nmon2,0.0);
                std::vector<double> Efdcp7(nmon2,0.0);
                std::vector<double> Efdcp8(nmon2,0.0);
                std::vector<double> Efdcp9(nmon2,0.0);

                // This will contain the 6 unique components of T that then will be
                // added to the gradients.
                std::vector<double> Efdtmpcp10(nmon2,0.0);
                std::vector<double> Efdtmpcp11(nmon2,0.0);
                std::vector<double> Efdtmpcp12(nmon2,0.0);
                std::vector<double> Efdtmpcp13(nmon2,0.0);
                std::vector<double> Efdtmpcp14(nmon2,0.0);
                std::vector<double> Efdtmpcp15(nmon2,0.0);

                std::vector<double> rx(nmon2,0.0);
                std::vector<double> ry(nmon2,0.0);
                std::vector<double> rz(nmon2,0.0);

                if (same) {
                for (size_t m2 = 0; m2 < m1; m2++) {
                    const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                      - xyz[fi_crd2 + jnmon23 + m2];
                    const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon22];
                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    // Some values that will be used in the screening functions
                    // TODO look at the exponential function intel vec

                    // Get screening functions
                    const double s1r3 = ri * risq;
                    rx[m2] = rijx * s1r3;
                    ry[m2] = rijy * s1r3;
                    rz[m2] = rijz * s1r3;
                    const double s2r5_3 = (3.0*s1r3)*risq;

                    const double s3r7_15 = (s2r5_3 * 5.0) * risq;
                    const double s3r7_15x2 = s3r7_15 * rijx2;
                    const double s3r7_15y2 = s3r7_15 * rijy2;
                    const double s3r7_15z2 = s3r7_15 * rijz2;

                    const double ts2x = s2r5_3 * rijx;
                    const double ts2y = s2r5_3 * rijy;
                    const double ts2z = s2r5_3 * rijz;
                
                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3 * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3 * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3 * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3 * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15 * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3 * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15 * rijy2 * rijy
                              - s2r5_3 * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3 * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3 * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3 * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                  }
                  for (size_t m2 = m1 + 1; m2 < nmon2; m2++) {
                    const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                      - xyz[fi_crd2 + jnmon23 + m2];
                    const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    // Some values that will be used in the screening functions
                    // TODO look at the exponential function intel vec

                    // Get screening functions
                    const double s1r3 = ri * risq;
                    rx[m2] = rijx * s1r3;
                    ry[m2] = rijy * s1r3;
                    rz[m2] = rijz * s1r3;
                    const double s2r5_3 = (3.0*s1r3)*risq;

                    const double s3r7_15 = (s2r5_3 * 5.0) * risq;
                    const double s3r7_15x2 = s3r7_15 * rijx2;
                    const double s3r7_15y2 = s3r7_15 * rijy2;
                    const double s3r7_15z2 = s3r7_15 * rijz2;

                    const double ts2x = s2r5_3 * rijx;
                    const double ts2y = s2r5_3 * rijy;
                    const double ts2z = s2r5_3 * rijz;
                     
                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3 * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3 * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3 * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3 * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15 * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3 * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15 * rijy2 * rijy
                              - s2r5_3 * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3 * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3 * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3 * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                  }
                } else {
                  for (size_t m2 = 0; m2 < nmon2; m2++) {
                    const double rijx = xyz[fi_crd1 + inmon13 + m1]
                                      - xyz[fi_crd2 + jnmon23 + m2];
                    const double rijy = xyz[fi_crd1 + inmon13 + m1 + nmon1]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon2];
                    const double rijz = xyz[fi_crd1 + inmon13 + m1 + nmon12]
                                      - xyz[fi_crd2 + jnmon23 + m2 + nmon22];

                    const double rijx2 = rijx*rijx;
                    const double rijy2 = rijy*rijy;
                    const double rijz2 = rijz*rijz;
                    const double rsq = rijx2 + rijy2 + rijz2;
                    const double r = std::sqrt(rsq);
                    const double ri = 1.0/r;
                    const double risq = ri*ri;
                    // Some values that will be used in the screening functions
                    // TODO look at the exponential function intel vec

                    // Get screening functions
                    const double s1r3 = ri * risq;
                    rx[m2] = rijx * s1r3;
                    ry[m2] = rijy * s1r3;
                    rz[m2] = rijz * s1r3;
                    const double s2r5_3 = (3.0*s1r3)*risq;

                    const double s3r7_15 = (s2r5_3 * 5.0) * risq;
                    const double s3r7_15x2 = s3r7_15 * rijx2;
                    const double s3r7_15y2 = s3r7_15 * rijy2;
                    const double s3r7_15z2 = s3r7_15 * rijz2;

                    const double ts2x = s2r5_3 * rijx;
                    const double ts2y = s2r5_3 * rijy;
                    const double ts2z = s2r5_3 * rijz;

                    Efdcp0[m2] = s3r7_15x2* rijx
                              - s2r5_3 * 3.0 * rijx;          // x x x
                    Efdcp1[m2] = s3r7_15x2 * rijy
                              - s2r5_3 * rijy;                // x x y
                    Efdcp2[m2] = s3r7_15x2 * rijz
                              - s2r5_3 * rijz;                // x x z
                    Efdcp3[m2] = s3r7_15y2 * rijx
                              - s2r5_3 * rijx;                // x y y
                    Efdcp4[m2] = s3r7_15 * rijx * rijy * rijz; // x y z
                    Efdcp5[m2] = s3r7_15z2 * rijx
                              - s2r5_3 * rijx;                // x z z
                    Efdcp6[m2] = s3r7_15 * rijy2 * rijy
                              - s2r5_3 * 3.0 * rijy;          // y y y
                    Efdcp7[m2] = s3r7_15y2 * rijz
                              - s2r5_3 * rijz;                // y y z
                    Efdcp8[m2] = s3r7_15z2 * rijy
                              - s2r5_3 * rijy;                // y z z
                    Efdcp9[m2] = s3r7_15z2 * rijz
                              - s2r5_3 * 3.0 * rijz;          // z z z

                    Efdtmpcp10[m2] = ts2x * rijx - s1r3; // alpha = x, beta = x
                    Efdtmpcp11[m2] = ts2x * rijy;         // alpha = x, beta = y
                    Efdtmpcp12[m2] = ts2x * rijz;         // alpha = x, beta = z
                    Efdtmpcp13[m2] = ts2y * rijy - s1r3; // alpha = y, beta = y
                    Efdtmpcp14[m2] = ts2y * rijz;         // alpha = y, beta = z
                    Efdtmpcp15[m2] = ts2z * rijz - s1r3; // alpha = z, beta = z
                  }
                }
                size_t shifti = fi_crd1 + 3*ns1*m1 + i3;
                size_t sii = fi_sites1 + ns1*m1 + i;
                for (size_t m2 = 0; m2 < nmon2; m2++) {
                  size_t shiftj = fi_crd2 + 3*ns2*m2 + j3;
                  size_t sij = fi_sites2 + ns2*m2 + j;
                  // chg-dip grad
                  grad[shifti] -=
                     (chg[sii] * Efdtmpcp10[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp10[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp11[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp11[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp12[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp12[m2] * mu[shifti + 2]);
                  grad[shifti + 1] -=
                     (chg[sii] * Efdtmpcp11[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp11[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp13[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp13[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp14[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp14[m2] * mu[shifti + 2]);
                  grad[shifti + 2] -=
                     (chg[sii] * Efdtmpcp12[m2] * mu[shiftj]
                    - chg[sij] * Efdtmpcp12[m2] * mu[shifti]
                    + chg[sii] * Efdtmpcp14[m2] * mu[shiftj + 1]
                    - chg[sij] * Efdtmpcp14[m2] * mu[shifti + 1]
                    + chg[sii] * Efdtmpcp15[m2] * mu[shiftj + 2]
                    - chg[sij] * Efdtmpcp15[m2] * mu[shifti + 2]);

                  // Update potential
                  // Sign due to distance
                  phi[sij] -= rx[m2]*mu[shifti] + ry[m2]*mu[shifti + 1]
                            + rz[m2]*mu[shifti + 2];

                  // dip-dip
                  grad[shifti] +=
                    (Efdcp0[m2]*mu[shifti]*mu[shiftj]           // x x x
                   + Efdcp1[m2]*mu[shifti]*mu[shiftj + 1]       // x x y
                   + Efdcp2[m2]*mu[shifti]*mu[shiftj + 2]       // x x z
                   + Efdcp1[m2]*mu[shifti + 1]*mu[shiftj]       // x y x
                   + Efdcp3[m2]*mu[shifti + 1]*mu[shiftj + 1]   // x y y
                   + Efdcp4[m2]*mu[shifti + 1]*mu[shiftj + 2]   // x y z
                   + Efdcp2[m2]*mu[shifti + 2]*mu[shiftj]       // x z x
                   + Efdcp4[m2]*mu[shifti + 2]*mu[shiftj + 1]   // x z y
                   + Efdcp5[m2]*mu[shifti + 2]*mu[shiftj + 2]); // x z z
                  grad[shifti + 1] +=
                    (Efdcp1[m2]*mu[shifti]*mu[shiftj]           // y x x
                   + Efdcp3[m2]*mu[shifti]*mu[shiftj + 1]       // y x y
                   + Efdcp4[m2]*mu[shifti]*mu[shiftj + 2]       // y x z
                   + Efdcp3[m2]*mu[shifti + 1]*mu[shiftj]       // y y x
                   + Efdcp6[m2]*mu[shifti + 1]*mu[shiftj + 1]   // y y y
                   + Efdcp7[m2]*mu[shifti + 1]*mu[shiftj + 2]   // y y z
                   + Efdcp4[m2]*mu[shifti + 2]*mu[shiftj]       // y z x
                   + Efdcp7[m2]*mu[shifti + 2]*mu[shiftj + 1]   // y z y
                   + Efdcp8[m2]*mu[shifti + 2]*mu[shiftj + 2]); // y z z
                  grad[shifti + 2] +=
                    (Efdcp2[m2]*mu[shifti]*mu[shiftj]           // z x x
                   + Efdcp4[m2]*mu[shifti]*mu[shiftj + 1]       // z x y
                   + Efdcp5[m2]*mu[shifti]*mu[shiftj + 2]       // z x z
                   + Efdcp4[m2]*mu[shifti + 1]*mu[shiftj]       // z y x
                   + Efdcp7[m2]*mu[shifti + 1]*mu[shiftj + 1]   // z y y
                   + Efdcp8[m2]*mu[shifti + 1]*mu[shiftj + 2]   // z y z
                   + Efdcp5[m2]*mu[shifti + 2]*mu[shiftj]       // z z x
                   + Efdcp8[m2]*mu[shifti + 2]*mu[shiftj + 1]   // z z y
                   + Efdcp9[m2]*mu[shifti + 2]*mu[shiftj + 2]); // z z z
                }
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

    fi_mon = 0;
    fi_sites = 0;
    fi_crd = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      std::string id = mon_id[fi_sites];

      // Redistribute gradients
      systools::RedistributeVirtGrads2Real(id, nmon, fi_crd, grad);

      // Gradients due to position dependant charges
      systools::ChargeDerivativeForce(id, nmon, fi_crd, fi_sites,
                               phi, grad, chg_grd);
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
      fi_crd += nmon * ns * 3;
    }

    
      

    return Eqq + Eind;
    
  } 

} // namespace elec
