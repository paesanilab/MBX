#include "sys_tools.h"

namespace systools {

//constants for water
const double gammaM = 0.426706882;
const double gamma1 = 1.0 - gammaM;
const double gamma2 = gammaM/2;
const double gamma21 = gamma2/gamma1;

std::vector<std::pair<std::string,size_t>> OrderMonomers
                   (std::vector<std::string> &mon, 
                    std::vector<size_t> &original_order) {
  std::vector<std::string> monomers = mon;
  mon.clear();
  original_order.clear();
  size_t n = monomers.size();
  while (n > 0) {
    std::string min_mon = monomers[0];
    size_t index_min = 0;
    for (size_t j = 1; j < monomers.size(); j++) {
      if (monomers[j] < min_mon && monomers[j] != "~") {
        min_mon = monomers[j];
        index_min = j;
      }
    }
    mon.push_back(min_mon);
    original_order.push_back(index_min);
    monomers[index_min] = "~";
    n--;
  }

  std::vector<std::pair<std::string,size_t>> mon_types_count;
  mon_types_count.push_back(std::make_pair(mon[0],1));
  size_t count = 0;
  std::string prev_mon = mon[0];
  for (size_t i = 1; i < mon.size(); i++) {
    if (mon[i] == prev_mon) {
      mon_types_count[count].second += 1;
    } else {
      prev_mon = mon[i];
      mon_types_count.push_back(std::make_pair(mon[i],1));
      count += 1;
    } 
  }

  return mon_types_count;

}

size_t SetUpMonomers(std::vector<std::string> mon, std::vector<size_t> &sites,
                     std::vector<size_t> &nat, std::vector<size_t> &fstind,
                     std::vector<double> &chg, std::vector<double> &pol,
                     std::vector<double> &polfac) {
  // Clearing Vectors
  sites.clear();
  nat.clear();
  fstind.clear();
  chg.clear();
  pol.clear();
  polfac.clear();
  
  size_t count = 0;
  for (size_t i = 0; i < mon.size(); i++) {
    if (mon[i] == "h2o") {
      // Filling things for water.
      // TODO: WARNING. For water, charges, and position of the M-site
      // depends on the 3 atom coordinate.  

      // Site Info
      // TODO Maybe we can read this from a database
      sites.push_back(4);
      nat.push_back(3);
      fstind.push_back(count);
      count += sites[i];

      // Charge info
      chg.push_back(0.0);
      chg.push_back(0.0);
      chg.push_back(0.0);
      chg.push_back(0.0);

      // Pol info
      pol.push_back(0.0);
      pol.push_back(0.0);
      pol.push_back(0.0);
      pol.push_back(0.0);

      // Polfac info
      polfac.push_back(0.0);
      polfac.push_back(0.0);
      polfac.push_back(0.0);
      polfac.push_back(0.0);
    } else {
      std::cerr << "No data in the dataset for monomer: "
                << mon[i] << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Setting total number of sites
  return count;
}

bool compare_pair (std::pair<size_t,double> a, std::pair<size_t,double> b) {
  return a.first < b.first;
}

void AddClusters(size_t n_max, double cutoff, size_t istart, size_t iend, 
                 size_t nmon, 
                 std::vector<double> xyz_orig,
                 std::vector<size_t> first_index, 
                 std::vector<size_t> &dimers, 
                 std::vector<size_t> &trimers) {

  // istart is the monomer position for which we will look all dimers and 
  // trimers that contain it. iend is the last monomer position.
  // This means, if istart is 0 and iend is 2, we will look for all dimers
  // and trimers that contain monomers 0 and/or 1. !!! 2 IS NOT INCLUDED. !!!

  // nmon is the number of monomers in xyz
  // xyz_orig is a double vector with positions of all atoms
  // first_index is a size_t vector with the first index of the site 'i'
  // in in the monomer vector
  // dimers and trimers will be filled with the dimers and trimers found

  // Obtain xyz vector with the positions of first atom of each monomer
  std::vector<double> xyz;
  for (size_t i = istart; i < nmon; i++) {
    xyz.push_back(xyz_orig[3*first_index[i]]);
    xyz.push_back(xyz_orig[3*first_index[i] + 1]);
    xyz.push_back(xyz_orig[3*first_index[i] + 2]);
  }

  // Obtain the data in the structure needed by the kd-tree
  kdtutils::PointCloud<double> ptc = kdtutils::XyzToCloud(xyz);

  // Build the tree
  typedef nanoflann::KDTreeSingleIndexAdaptor<
    nanoflann::L2_Simple_Adaptor<double, kdtutils::PointCloud<double>>,
    kdtutils::PointCloud<double>, 3 /* dim */> my_kd_tree_t;
  my_kd_tree_t index(3 /*dim*/, ptc,
    nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
  index.buildIndex();

  // Perform a radial search within the cutoff
  dimers.clear();
  if (n_max > 2)
    trimers.clear();

  std::vector<size_t> idone;
  for (size_t i = 0; i < iend - istart; i++) {
    // Define the query point
    double point[3];
    point[0] = ptc.pts[i].x;
    point[1] = ptc.pts[i].y;
    point[2] = ptc.pts[i].z;

    // Perform the search
    std::vector<std::pair<size_t, double>> ret_matches;
    nanoflann::SearchParams params;
    const size_t nMatches = index.radiusSearch(point,
      cutoff * cutoff, ret_matches, params);

    std::sort(ret_matches.begin(), ret_matches.end(), compare_pair);
    std::set<std::pair<size_t, size_t>> donek;

    // Add the pairs that are not in the dimer vector
    for (size_t j = 0; j < nMatches; j++) {
      if (ret_matches[j].first > i) {
        dimers.push_back(i + istart);
        dimers.push_back(ret_matches[j].first + istart);

        // Add trimers if requested
        if (n_max > 2) {
          for (size_t k = 0; k < nMatches; k++) {
            if (ret_matches[k].first > ret_matches[j].first) {
              trimers.push_back(i + istart);
              trimers.push_back(ret_matches[j].first + istart);
              trimers.push_back(ret_matches[k].first + istart); 
              donek.insert(std::make_pair(ret_matches[j].first,
                                          ret_matches[k].first));
            } 
          }
          // Define query point, which is each of the points 'j' inside the 
          // radius of 'i' 
          double point2[3];
          point2[0] = ptc.pts[ret_matches[j].first].x;
          point2[1] = ptc.pts[ret_matches[j].first].y;
          point2[2] = ptc.pts[ret_matches[j].first].z;
          std::vector<std::pair<size_t, double>> ret_matches2;
          nanoflann::SearchParams params2;
          const size_t nMatches2 = index.radiusSearch(point2,
            cutoff * cutoff, ret_matches2, params2);
          
          std::sort(ret_matches2.begin(), ret_matches2.end(), compare_pair);

          // Add the trimers that fulfil i > j > k, to avoid double counting
          // We will add all trimers that fulfill the condition:
          // At least 2 of the three distances must be smaller than the cutoff 
          std::pair<std::set<std::pair<size_t,size_t>>::iterator,bool> ret;
          for (size_t k = 0; k < nMatches2; k++) {
            if (ret_matches2[k].first > i) {
              size_t jel = ret_matches[j].first;
              size_t kel = ret_matches2[k].first;
              if (ret_matches[j].first > ret_matches2[k].first) {
                jel = ret_matches2[k].first;
                kel = ret_matches[j].first;
              }
              ret = donek.insert(std::make_pair(jel,kel));
              if (ret.second && kel > jel) {
                trimers.push_back(i + istart);
                trimers.push_back(jel + istart);
                trimers.push_back(kel + istart);
              }
            }
          }
        }
      }
    }
  }

}

void GetExcluded(std::string mon, 
                 excluded_set_type &exc12,
                 excluded_set_type &exc13,
                 excluded_set_type &exc14) {

  // Clearing excluded pairs just in case
  exc12.clear();
  exc13.clear();
  exc14.clear();

  if (mon == "h2o") {
    // 12 distances
    exc12.insert(std::make_pair(0,1));
    exc12.insert(std::make_pair(0,2));
    exc12.insert(std::make_pair(0,3));
//    exc12.insert(std::make_pair(1,3));
//    exc12.insert(std::make_pair(2,3));
    // 13 distances
    exc13.insert(std::make_pair(1,2));
    exc13.insert(std::make_pair(1,3));
    exc13.insert(std::make_pair(2,3));
  }
}

bool IsExcluded(excluded_set_type exc, size_t a, size_t b) {
  return ((exc.find(std::make_pair(a, b)) != exc.end()) ||
          (exc.find(std::make_pair(b, a)) != exc.end()));
}

double GetAdd(bool is12, bool is13, bool is14, std::string mon) {
  double aDD = 0.055;
  if (mon == "h2o") {
    if (is12) {
      aDD = 0.626;
    } else {
      aDD = 0.055;
    }
  } else {
    if (is12 || is13) {
      aDD = 0.3;
    } else {
      aDD = 0.055;
    }
  }
  
  return aDD;
}


void SetVSites (std::vector<double> &xyz, std::string mon_id,
    size_t n_mon, size_t nsites, size_t fst_ind) {
  
  size_t fstind_3 = 3*fst_ind;

  if (mon_id == "h2o") {

    // Some useful constants
    size_t nmns = n_mon*3;
    size_t nmns2 = nmns*2;
    size_t nmns3 = nmns*3;

    // Reorganize data so is contigious
    std::vector<double> xyz2(nmns*nsites,0.0);
    for (size_t nv = 0; nv < n_mon; nv++) {
      size_t nvns3 = nv*nsites*3;
      for (size_t i = 0; i < nsites; i++) {
        size_t i3 = 3*i;
        size_t i3nm = i3*n_mon;
        for (size_t j = 0; j < 3; j++) {
          xyz2[nv + j*n_mon + i3nm] = xyz[nvns3 + i3 + j + fstind_3];
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
    for (size_t nv = 0; nv < n_mon; nv++) {
      size_t nvns3 = nv*nsites*3;
      for (size_t j = 0; j < 3; j++) {
        xyz[nvns3 + 9 + j + fstind_3] = xyz2[nv + j*n_mon + 9*n_mon];
      }
    }
  }
}

void SetCharges (std::vector<double> xyz, std::vector<double> &charges,
    std::string mon_id, size_t n_mon, size_t nsites,
    size_t fst_ind) {

  if (mon_id == "h2o") {
    // chgtmp = M, H1, H2 according to ttm4.
    std::vector<double> chgtmp;
    size_t fstind_3 = 3*fst_ind;

    // Calculate individual monomer's charges
    for (size_t nv = 0; nv < n_mon; nv++) {

      size_t ns3 = nsites*3;
      
      // Getting front and end of xyz vector of 1 monomer in system
      std::vector<double>::const_iterator first = xyz.begin()+(nv*ns3)+fstind_3;
      std::vector<double>::const_iterator end = 
                                             xyz.begin()+((nv+1)*ns3)+fstind_3;
      std::vector<double> atomcoords(first, end);
      std::vector<double> chgtmpnv((nsites-1));

      ps::dms_nasa (0.0, 0.0, 0.0, atomcoords.data(), chgtmpnv.data(), 0, false);
      // Inserting the found charges into chgtmp vector before calculating
      // new charge values
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

      // looping over M
      chg2[nv + 3*n_mon] = chgtmp[nv*3];
    }

    std::vector<double> chg2temp = chg2;

    // calculating charge
    for (size_t nv = 0; nv < n_mon; nv++) {
      size_t hy1 = n_mon + nv;
      size_t hy2 = 2*n_mon + nv;
      size_t msite = 3*n_mon + nv;

      chg2[hy1] = CHARGECON*(chg2temp[hy1] + 
                             gamma21*(chg2temp[hy1]+chg2temp[hy2]));
      chg2[hy2] = CHARGECON*(chg2temp[hy2] +
                             gamma21*(chg2temp[hy1]+chg2temp[hy2]));
      chg2[msite] = CHARGECON*(chg2temp[msite]/(1.0-gammaM));
    }

    // Return all coordinates to the original vector
    for (size_t nv = 0; nv < n_mon; nv++) {
      for (size_t j = 0; j < nsites; j++) {
        charges[nv*nsites + j + fst_ind] = chg2[nv + n_mon*j];
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
      polfac[fst_ind + (nv*nsites)+3] = 1.310;
    }
  }
}

void SetPol (std::vector<double> &pol, 
    std::string mon_id, size_t n_mon, size_t nsites, size_t fst_ind){

  if (mon_id == "h2o") {
    for (size_t nv = 0; nv < n_mon; nv++) {
      pol[fst_ind + nv*nsites] = 1.310;
      pol[fst_ind + (nv*nsites)+1] = 0.294;
      pol[fst_ind + (nv*nsites)+2] = 0.294;
      pol[fst_ind + (nv*nsites)+3] = 0.0;
    }

  }
}





} //systools
