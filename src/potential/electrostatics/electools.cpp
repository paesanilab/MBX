#include <iostream>
#include <vector>
#include <string>

#include "electools.h"
#include "ps.h"

namespace elec {

  //constants for water
  const double gammaM = 0.426706882;
  const double gamma1 = 1.0 - gammaM;
  const double gamma2 = gammaM/2;
  const double gamma21 = gamma2/gamma1;

  void SetVSites (std::vector<double> &xyz, std::string mon_id,
      size_t n_mon, size_t nsites, size_t fst_ind) {

    if (mon_id == "h2o") {

      // Some useful constants
      size_t nmns = n_mon*3;
      size_t nmns2 = nmns*2;
      size_t nmns3 = nmns*3;

      // Reorganize data so is contigious
      std::vector<double> xyz2(nmns*nsites,0.0);
      for (size_t nv = fst_ind; nv < n_mon+fst_ind; nv++) {
        size_t nvns3 = nv*nsites*3;
        for (size_t i = 0; i < nsites; i++) {
          size_t i3 = 3*i;
          size_t i3nm = i3*n_mon;
          for (size_t j = 0; j < 3; j++) {
            xyz2[nv + j*n_mon + i3nm] = xyz[nvns3 + i3 + j];
          } 
        }
      }

      // TODO Check why is multiversioning. Should not.
      // Obtain M-site coordinates from different sites for water
      for (size_t i = 0; i < 3; i++) {
        size_t inm = i*n_mon;
        for (size_t nv = 0; nv < n_mon; nv++) {
          xyz2[nmns3 + inm + nv] = gamma1 * xyz2[0 + inm + nv]
            + gamma2*(xyz2[nmns + inm + nv] + xyz2[nmns2 + inm + nv]);
        }
      }

      // Return the M-site coordinates to the original xyz vector
      for (size_t nv = fst_ind; nv < n_mon+fst_ind; nv++) {
        size_t nvns3 = nv*nsites*3;
        for (size_t j = 0; j < 3; j++) {
          xyz[nvns3 + 9 + j] = xyz2[nv + j*n_mon + 9*n_mon];
        }
      }
    }
  }

  void SetCharges (std::vector<double> &xyz, std::vector<double> &charges,
      std::string mon_id, size_t n_mon, size_t nsites,
      size_t fst_ind) {

    if (mon_id == "h2o") {
      // chgtmp = M, H1, H2 according to ttm4.
      std::vector<double> chgtmp;

      // Calculate individual monomer's charges TODO deal with fst_ind
      for (size_t nv = fst_ind; nv < n_mon+fst_ind; nv++) {
        size_t ns3 = nsites*3;
        std::vector<double>::const_iterator first = xyz.begin()+(nv*ns3)+fst_ind;
        std::vector<double>::const_iterator end = xyz.begin()+((nv+1)*ns3)+fst_ind;
        std::vector<double> atomcoords(first, end);
        std::vector<double> chgtmpnv((nsites-1));

        ps::dms_nasa (0.0, 0.0, 0.0, atomcoords.data(), chgtmpnv.data(), 0, false);
        chgtmp.insert (chgtmp.end(), chgtmpnv.begin(), chgtmpnv.end());
      }

      const double CHARGECON = constants::CHARGECON;

      // Reorganize 
      std::vector<double> chg2(n_mon*nsites,0.0);
      // looping over number of monomers
      for (size_t nv = 0; nv < n_mon; nv++) {
        // looping over sites -- H1 and H2
        for (size_t i = 1; i < nsites - 1; i++) {
          chg2[nv + i*n_mon] = chgtmp[i+nv*(nsites-1)];
        }

        // looping over M, since M is in the beginning of chgtmp buggy??
        chg2[nv + 3*n_mon] = chgtmp[nv*3];
      }

      std::vector<double> chg2temp = chg2;

      // calculating charge
      for (size_t nv = 0; nv < n_mon; nv++) {
        size_t hy1 = n_mon + nv;
        size_t hy2 = 2*n_mon + nv;
        size_t msite = 3*n_mon + nv;       

        chg2[hy1] = CHARGECON*(chg2temp[hy1] + gamma21*(chg2temp[hy1]+chg2temp[hy2]));
        chg2[hy2] = CHARGECON*(chg2temp[hy2] + gamma21*(chg2temp[hy1]+chg2temp[hy2]));
        chg2[msite] = CHARGECON*(chg2temp[msite]/(1.0-gammaM));
      }

      // Return all coordinates to the original vector
      for (size_t nv = fst_ind; nv < n_mon+fst_ind; nv++) {
        for (size_t j = 0; j < nsites; j++) {
          charges[nv*nsites + j] = chg2[nv + n_mon*j];
        }
      }
    }
  }

  void SetPolfac (std::vector<double> &polfac, std::string mon_id, 
      size_t n_mon, size_t nsites, size_t fst_ind){

    if (mon_id == "h2o") {
      // edit
      for (size_t nv = 0; nv < n_mon; nv++) {
        polfac[fst_ind + nv*nsites] = 1.310;
        polfac[fst_ind + (nv*nsites)+1] = 0.294;
        polfac[fst_ind + (nv*nsites)+2] = 0.294;
        polfac[fst_ind + (nv*nsites)+3] = 0.0;
      }
    }  
  }

  void SetPol (std::vector<double> &atmpolar, std::vector<double> &polfac,
      std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind){

    if (mon_id == "h2o") {
      SetPolfac (polfac, mon_id, n_mon, nsites, fst_ind);
      for (size_t i = fst_ind; i < n_mon + fst_ind; i++) {
        atmpolar[i] = polfac[i];
      }

      // M using O polfac. Seen in set_pol of ttm4
      for (size_t nv = 0; nv < n_mon; nv++) {
        polfac[fst_ind + (nv*nsites) + 3] = polfac[fst_ind + nv*nsites];
      }
    }
  }

} //elec
