#include "potential/electrostatics/electrostatics.h"

namespace elec {

  double electrostatics(const std::vector<double> chg,               
    const std::vector<double> polfac, 
    const std::vector<double> pol, 
    const std::vector<double> orig_xyz, 
    const std::vector<std::string> mon_id, 
    const std::vector<size_t> sites, 
    const std::vector<size_t> first_ind,
    const excluded_set_type& excluded12, 
    const excluded_set_type& excluded13, 
    const excluded_set_type& excluded14, 
    const std::vector<std::pair<std::string,size_t>> mon_type_count, 
    const bool do_grads, 
    std::vector<double> &grad) {

    // Damping declarations
    const double aCC = 0.4;
    const double aCD = 0.4;
    const double aDD = 0.055;

    // Constants that will be used later
    const double g34 = std::exp(gammln(3.0/4.0));

    // System properties and sizes
    const size_t nsites = chg.size();
    std::vector<double> sqrtpol(3*nsites, 0.0);

    // Electric fields and potential
    std::vector<double> Efq(3*nsites,0.0);
    std::vector<double> Efd(3*nsites,0.0);
    std::vector<double> phi(nsites,0.0);
    
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
    std::vector<double> xyz(nsites,0.0);
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
    // Monomers alone
    fi_mon = 0;
    size_t fi_sites = 0;
    for (size_t mt = 0; mt < mon_type_count.size(); mt++) {
      size_t ns = sites[fi_mon];
      size_t nmon = mon_type_count[mt].second;
      size_t nmon2 = nmon*2;
      for (size_t i = 0; i < ns -1; i++) {
        size_t inmon  = i*nmon;
        size_t inmon3  = 3*inmon;
        size_t i3 = 3*i;
        for (size_t j = i + 1; j < ns; j++) {
          size_t jnmon  = j*nmon;
          size_t jnmon3  = 3*jnmon;
          size_t j3 = 3*j;
          // TODO For now, assuming A > std::numeric_limits<double>::epsilon();
          // TODO COntinue only if i and j are not bonded
          double A = polfac[fi_sites + i] * polfac[fi_sites + j];
          double Ai = 1/A;
          double Asqsq = A*A*A*A;
          for (size_t m = 0; m < nmon; m++) {
            //  Distances and values that will be reused
            const double rijx = xyz[inmon3 + m] - xyz[jnmon3 + m];
            const double rijy = xyz[inmon3 + m + nmon] - xyz[jnmon3 + m + nmon];
            const double rijz = xyz[inmon3 + m + nmon2] - xyz[jnmon3 + m + nmon2];
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
            const double a_mrt = std::pow(aCC, 1.0/4.0);
            //const double a4 = aCC * 4.0;

            // Get screening functions
            const double s1r = ri - exp1*ri;
            const double s0r = s1r + a_mrt * Ai * g34 * gq;
            const double s1r3 = s1r * risq;

            // Update the field
            const size_t shift = first_ind[fi_mon + m];
            const size_t shift3 = 3*shift;
            const size_t spi = shift + i;
            const size_t spj = shift + j;
        
            phi[i] += s0r *chg[spj];
            phi[j] += s0r *chg[spi];
     
            // Update Electric field
            Efq[shift3 + i3] += s1r3 * chg[spj] * rijx;
            Efq[shift3 + j3] -= s1r3 * chg[spi] * rijx;
            Efq[shift3 + i3 + 1] += s1r3 * chg[spj] * rijy;
            Efq[shift3 + j3 + 1] -= s1r3 * chg[spi] * rijy;
            Efq[shift3 + i3 + 2] += s1r3 * chg[spj] * rijz;
            Efq[shift3 + j3 + 2] -= s1r3 * chg[spi] * rijz;
          }
        }
      }
      
      // Update first indexes
      fi_mon += nmon;
      fi_sites += nmon * ns;
    }
    
    

    return 0.0;
  
  } 

} // namespace elec
