#include "system.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {initialized_ = false;}
System::~System() {}

size_t System::GetNumMol() {return nmol_;}
size_t System::GetNumSites() {return nsites_;}
size_t System::GetMonNat(size_t n) {return nat_[n];}
size_t System::GetFirstInd(size_t n) {return first_index_[n];}

std::vector<size_t> System::GetDimers() {return dimers_;}
std::vector<size_t> System::GetTrimers() {return trimers_;}
std::vector<size_t> System::GetMolecule(size_t n) {return molecules_[n];}
std::vector<std::string> System::GetSysAtNames() {return atoms_;}
std::vector<double> System::GetSysXyz() {return xyz_;}

std::string System::GetMonId(size_t n) {return monomers_[n];}

void System::SetSysXyz(std::vector<double> xyz) {
  // TODO Check that sizes are the same
  std::copy(xyz.begin(), xyz.end(), xyz_.begin());
}

void System::AddMonomer(std::vector<double> xyz, 
             std::vector<std::string> atoms, std::string id){
  // Adding coordinates
  for (auto i = xyz.begin() ; i != xyz.end() ; i++)
    xyz_.push_back(*i);
  // Adding atom names
  for (auto i = atoms.begin() ; i != atoms.end() ; i++)
    atoms_.push_back(*i);
  // Adding id
  monomers_.push_back(id);
}

void System::AddMolecule(std::vector<size_t> molec) {
  molecules_.push_back(molec);
}

void System::Initialize() {
  if (initialized_) return;

  cutoff_ = 10.0;
  
  AddMonomerInfo();
  nmol_ = molecules_.size();
  nmon_ = monomers_.size();
  AddClusters(3);
  // TODO Here should go the order and rearrengement stuff
}

void System::AddMonomerInfo() {
  // TODO Add check if empty or not initialized
  std::vector<double> xyz = xyz_;
  xyz_.clear();

  std::vector<std::string> atoms = atoms_;
  atoms_.clear();

  // TODO Sort the monomers and put them together by type
  mon_type_count_ = systools::OrderMonomers(monomers_, initial_order_); 

  // At this point, the monomers are sorted by name.
  // TODO maybe use a swich would be better than if else
  // TODO maybe we should try to find out number of sites first and reserve
  // memory, rather than pushing back?
  
  nsites_ = systools::SetUpMonomers(monomers_, sites_, nat_, first_index_,
                          chg_, pol_, polfac_);
  
  // Rearranging coordinates to account for virt sites
  xyz_ = std::vector<double> (3*nsites_, 0.0);
  atoms_ = std::vector<std::string> (nsites_, "virt");
  size_t count = 0;
    
  for (size_t i = 0; i < monomers_.size(); i++) {
    //size_t k = initial_order_[i];
    std::copy(xyz.begin() + 3 * count,
              xyz.begin() + 3 * (nat_[i] + count),
              xyz_.begin() + 3 * first_index_[i]);
    std::copy(atoms.begin() + count,
              atoms.begin() + nat_[i] + count,
              atoms_.begin() + first_index_[i]);
    count += nat_[i];
  }

  // Setting Gradients to 0
  grd_ = std::vector<double> (3*nsites_, 0.0);
  

}

void System::AddClusters(size_t n_max) {
  // Obtain xyz vector with the positions of first atom of each monomer
  std::vector<double> xyz;
  for (size_t i = 0; i < monomers_.size(); i++) {
    xyz.push_back(xyz_[3*first_index_[i]]);
    xyz.push_back(xyz_[3*first_index_[i] + 1]);
    xyz.push_back(xyz_[3*first_index_[i] + 2]);
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
  dimers_.clear();
  trimers_.clear();
  for (size_t i = 0; i < monomers_.size(); i++) {
    // Define the query point
    double point[3];
    point[0] = ptc.pts[i].x;
    point[1] = ptc.pts[i].y;
    point[2] = ptc.pts[i].z;
    
    // Perform the search
    std::vector<std::pair<size_t, double>> ret_matches;
    nanoflann::SearchParams params;
    const size_t nMatches = index.radiusSearch(point,
      cutoff_ * cutoff_, ret_matches, params);

    // Add the pairs that are not in the dimer vector
    for (size_t j = 0; j < nMatches; j++) {
      if (ret_matches[j].first > i) {
        dimers_.push_back(i);
        dimers_.push_back(ret_matches[j].first);
      
        
        // Add trimers if requested
        if (n_max > 2) {
          // Define query point, which is each of the points 'j' inside the 
          // radius of 'i' 
          double point2[3];
          point2[0] = ptc.pts[ret_matches[j].first].x;
          point2[1] = ptc.pts[ret_matches[j].first].y;
          point2[2] = ptc.pts[ret_matches[j].first].z;
          std::vector<std::pair<size_t, double>> ret_matches2;
          nanoflann::SearchParams params2;
          const size_t nMatches2 = index.radiusSearch(point2,
            cutoff_ * cutoff_, ret_matches2, params2);
 
          // Add the trimers that fulfil i > j > k, to avoid double counting
          // We will add all trimers that fulfill the condition:
          // At least 2 of the three distances must be smaller than the cutoff 
          for (size_t k = 0; k < nMatches2; k++) {
            if (ret_matches2[k].first > ret_matches[j].first) {
              trimers_.push_back(i);
              trimers_.push_back(ret_matches[j].first);
              trimers_.push_back(ret_matches2[k].first);
            }
          }
        }
      }
    }
  }
  
}

double System::Energy(std::vector<double> &grd, bool do_grads) {

  // 1B ENERGY
  // Loop overall the monomers and get their energy
  size_t cmt = 0;
  size_t ccrd = 0;
  energy_ = 0.0;
  std::fill(grd.begin(), grd.end(), 0.0);
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    // Useful variables
    std::vector<double> energy = std::vector<double>
                                 (mon_type_count_[k].second, 0.0);
    size_t ncoord = 3 * nat_[cmt] * mon_type_count_[k].second;
    // XYZ with real sites
    std::vector<double> xyz = std::vector<double> (ncoord,0.0);
    
    // Set up real coordinates
    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
      std::copy(xyz_.begin() + ccrd + 3 * i * sites_[cmt],
                xyz_.begin() + ccrd + 3 * (i * sites_[cmt] + nat_[cmt]),
                xyz.begin() + 3 * i * nat_[cmt]);
    }

    double grd2[ncoord];
    std::fill(grd2, grd2 + ncoord, 0.0);
    // Get energy of the chunk as function of monomer
    if (mon_type_count_[k].first == "h2o") {
      if (do_grads)  {
        energy = ps::pot_nasa(xyz.data(), grd2, mon_type_count_[k].second);
      } else {
        energy = ps::pot_nasa(xyz.data(), 0, mon_type_count_[k].second);
      }
    } else {
      std::fill(energy.begin(), energy.end(),0.0);
    }

    // Add energy to energy_
    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
      energy_ += energy[i];

      if (do_grads) {
        // Reorganize gradients
        for (size_t j = 0; j < 3*nat_[cmt]; j++) {
          grd_[ccrd + 3*i*sites_[cmt] + j] += grd2[3*i*nat_[cmt] + j];
        }
      }
    }
    
    // Update ccrd and cmt
    ccrd += 3 * mon_type_count_[k].second * sites_[cmt];
    cmt += mon_type_count_[k].second;
  }

  // 2B ENERGY
  double e2b = 0;
  
  // Dimers are ordered
  // TODO put this in chunk so can be vectorized
  // TODO Maybe initialize here all possible dimer structs?
//  for (size_t i = 0; i < dimers_.size(); i+=2) {
//    if (monomers_[dimers_[i]] == "h2o" and
//        monomers_[dimers_[i + 1]] == "h2o") {
//      x2o::x2b_v9x pot;
//      if (do_grads) {
//        double grdx[18];
//        std::fill(grdx, grdx + 18, 0.0);
//        e2b += pot.eval(xyz_.data() + 3*first_index_[dimers_[i]],
//                        xyz_.data() + 3*first_index_[dimers_[i + 1]],
//                        grdx, grdx + 9);
//        for (size_t j = 0; j < 9; j++) {
//          grd_[3*first_index_[dimers_[i]] + j] += grdx[j];
//          grd_[3*first_index_[dimers_[i + 1]] + j] += grdx[j + 9];
//        }
//      } else {
//        e2b += pot.eval(xyz_.data() + 3*first_index_[dimers_[i]],
//                        xyz_.data() + 3*first_index_[dimers_[i + 1]]);
//      }
//    }
//  }
  // Vectorizable part
  // TODO UNCOMMENT
  size_t nd = 0;
  std::vector<double> xyz1;
  std::vector<double> xyz2;
  std::vector<double> grd1;
  std::vector<double> grd2;
  std::string m1;
  std::string m2;
  
  // First dimer if it exists
  if (dimers_.size() > 0) {
    for (size_t j = 0; j < 3*nat_[dimers_[0]]; j++) {
      xyz1.push_back(xyz_[3*first_index_[dimers_[0]] + j]);
      grd1.push_back(0.0);
    }
    for (size_t j = 0; j < 3*nat_[dimers_[1]]; j++) {
      xyz2.push_back(xyz_[3*first_index_[dimers_[1]] + j]);
      grd2.push_back(0.0);
    }
    m1 = monomers_[dimers_[0]];
    m2 = monomers_[dimers_[1]];
    nd++;
  }

  // Rest of dimers
  for (size_t i = 2; i < dimers_.size(); i+=2) {
    // If one of the monomers is different as the previous one
    // since dimers are also ordered, means that no more dimers of that
    // type exist. Thus, do calculation, update m? and clear xyz
    if (monomers_[dimers_[i]] != m1 ||
        monomers_[dimers_[i + 1]] != m2) {
      if (do_grads) {
        e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2, grd1, grd2);
        for (size_t k = 0; k < nd ; k++) {
          for (size_t j = 0; j < 3*nat_[dimers_[i - 2*k]]; j++) {
            grd_[3*first_index_[dimers_[i - 2*k]] + j] 
                += grd1[(nd - k - 1)*3*nat_[dimers_[i - 2*k]] + j];
          }
          for (size_t j = 0; j < 3*nat_[dimers_[i- 2*k +1]]; j++) {
            grd_[3*first_index_[dimers_[i -2*k + 1]] + j] 
                += grd2[(nd - k - 1)*3*nat_[dimers_[i - 2*k + 1]] + j];
          }
        }
      } else {
        e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2);
      }
      nd = 0;
      xyz1.clear();
      xyz2.clear();
      grd1.clear();
      grd2.clear();
      m1 = monomers_[dimers_[i]];
      m2 = monomers_[dimers_[i + 1]];
    }

    // Push the coordinates
    for (size_t j = 0; j < 3*nat_[dimers_[i]]; j++) {
      xyz1.push_back(xyz_[3*first_index_[dimers_[i]] + j]);
      grd1.push_back(0.0);
    }
    for (size_t j = 0; j < 3*nat_[dimers_[i+1]]; j++) {
      xyz2.push_back(xyz_[3*first_index_[dimers_[i+1]] + j]);
      grd2.push_back(0.0);
    }
    
    nd++;
  }

  // Last Iteration is not done
  if (dimers_.size() > 0) {
    if (do_grads) {
      e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2, grd1, grd2);
      for (size_t k = 0; k < nd ; k++) {
        for (size_t j = 0; j < 3*nat_[dimers_[dimers_.size()-2*k-2]]; j++) {
          grd_[3*first_index_[dimers_[dimers_.size()-2*k-2]]+j] 
              += grd1[(nd - k - 1)*3*nat_[dimers_[dimers_.size()
                 - 2*k - 2]] + j];
        }
        for (size_t j = 0; j < 3*nat_[dimers_[dimers_.size()-2*k-1]]; j++) {
          grd_[3*first_index_[dimers_[dimers_.size()-2*k-1]]+j] 
              += grd2[(nd - k - 1)*3*nat_[dimers_[dimers_.size() 
                 - 2*k - 1]] + j];
        }
      }
    } else {
      e2b += e2b::get_2b_energy(m1, m2, nd, xyz1, xyz2);
    }
    xyz1.clear();
    xyz2.clear();
    grd1.clear();
    grd2.clear();
  }

  energy_ += e2b;

  // 3B ENERGY
  double e3b = 0;
  // trimers are ordered
  // TODO put this in chunk so can be vectorized
  // TODO Maybe initialize here all possible trimer structs?
  for (size_t i = 0; i < trimers_.size(); i+=3) {
    if (monomers_[trimers_[i]] == "h2o" and
        monomers_[trimers_[i + 1]] == "h2o" and
        monomers_[trimers_[i + 2]] == "h2o") {
      x2o::x3b_v2x pot;
      if (do_grads) {
        double grdx[27];
        std::fill(grdx, grdx + 27, 0.0);
        e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
                        xyz_.data() + 3*first_index_[trimers_[i + 1]],
                        xyz_.data() + 3*first_index_[trimers_[i + 2]],
                        grdx, grdx + 9, grdx + 18);
        for (size_t j = 0; j < 9; j++) {
          grd_[3*first_index_[trimers_[i]] + j] += grdx[j];
          grd_[3*first_index_[trimers_[i + 1]] + j] += grdx[j + 9];
          grd_[3*first_index_[trimers_[i + 2]] + j] += grdx[j + 18];
        }
      } else {
        e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
                        xyz_.data() + 3*first_index_[trimers_[i + 1]],
                        xyz_.data() + 3*first_index_[trimers_[i + 2]]);
      }
    }
  }

  energy_ += e3b;

  grd.clear();
  grd = grd_;

  return energy_;
}

//double System::Energy(std::vector<double> &grd) {
//  // 1B ENERGY
//  // Loop overall the monomers and get their energy
//  size_t cmt = 0;
//  size_t ccrd = 0;
//  energy_ = 0.0;
//  for (size_t k = 0; k < mon_type_count_.size(); k++) {
//    // Useful variables
//    std::vector<double> energy;
//    size_t ncoord = 3 * nat_[cmt] * mon_type_count_[k].second;
//    // XYZ with real sites
//    double xyz[ncoord];
//    
//    // Set up real coordinates
//    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
//      std::copy(xyz_.begin() + ccrd + 3 * i * sites_[cmt],
//                xyz_.begin() + ccrd + 3 * (i+1) * nat_[cmt],
//                xyz + 3 * i * nat_[cmt]);
//    }
//
//    // Get energy of the chunk as function of monomer
//    double grd2[ncoord];
//    std::fill(grd2, grd2 + ncoord, 0.0);
//    if (mon_type_count_[k].first == "h2o") {
//      energy = ps::pot_nasa(xyz, grd2, mon_type_count_[k].second);
//    } else {
//      std::fill(energy.begin(), energy.end(),0.0);
//    }
//
//    // Add energy to energy_
//    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
//      energy_ += energy[i];
//
//      // Reorganize gradients
//      for (size_t j = 0; j < 3*nat_[cmt]; j++) {
//        grd_[ccrd + 3*i*sites_[cmt] + j] += grd2[3*i*nat_[cmt] + j];
//      }
//    }
//
//    energy.clear();
//    
//    // Update ccrd and cmt
//    ccrd += 3 * mon_type_count_[k].second * sites_[cmt];
//    cmt += mon_type_count_[k].second;
//  }
//
//  // 2B ENERGY
//  bool do_calc = false;
//  size_t ni = 0;
//  size_t nj = 0;
//  double e2b = 0;
//
//  // Dimers are ordered
//  // TODO put this in chunk so can be vectorized
//  // TODO Maybe initialize here all possible dimer structs?
//  for (size_t i = 0; i < dimers_.size(); i+=2) {
//    if (monomers_[dimers_[i]] == "h2o" and
//        monomers_[dimers_[i + 1]] == "h2o") {
//      double grdx[18];
//      std::fill(grdx, grdx + 18, 0.0);
//      x2o::x2b_v9x pot;
//      e2b += pot.eval(xyz_.data() + 3*first_index_[dimers_[i]],
//                      xyz_.data() + 3*first_index_[dimers_[i + 1]],
//                      grdx, grdx + 9);
//      for (size_t j = 0; j < 9; j++) {
//        grd_[3*first_index_[dimers_[i]] + j] += grdx[j];
//        grd_[3*first_index_[dimers_[i + 1]] + j] += grdx[j + 9];
//      }
//    }
//  }

  // Vectorizable part
  // TODO UNCOMMENT
//  size_t nd = 0;
//  std::vector<double> xyz1;
//  std::vector<double> xyz2;
//  std::vector<double> grd1;
//  std::vector<double> grd2;
//  std::string m1;
//  std::string m2;
//  
//  // First dimer if it exists
//  if (dimers_.size() > 0) {
//    for (size_t j = 0; j < nat_[3*first_index_[dimers_[0]]]; j++) {
//      grd1.push_back(0.0);
//      xyz1.push_back(xyz_[3*first_index_[dimers_[0]] + j]);
//    }
//    for (size_t j = 0; j < nat_[3*first_index_[dimers_[1]]]; j++) {
//      grd2.push_back(0.0);
//      xyz2.push_back(xyz_[3*first_index_[dimers_[1]] + j]);
//    }
//    m1 = monomers_[3*first_index_[dimers_[0]]];
//    m2 = monomers_[3*first_index_[dimers_[1]]];
//    nd++;
//  }
//
//  // Rest of dimers
//  for (size_t i = 2; i < dimers_.size(); i+=2) {
//    // If one of the monomers is different as the previous one
//    // since dimers are also ordered, means that no more dimers of that
//    // type exist. Thus, do calculation, update m? and clear xyz
//    if (monomers_[3*first_index_[dimers_[i]]] != m1 ||
//        monomers_[3*first_index_[dimers_[i + 1]]] != m2) {
//      e2b += get_2b_energy(m1, m2, nd, xyz1, xyz2, grd1, grd2)
//      // Relocate gradients
//      for (size_t j = i/2 - nd; j < i/2; j++) {
//        for (size_t k = 0; k < nat_[dimers_[2*j]])
//          grd_[3*first_index_[dimers_[2*j]] + k] += grd1[k];
//        for (size_t k = 0; k < nat_[dimers_[2*j + 1]])
//          grd_[3*first_index_[dimers_[2*j + 1]] + k] += grd2[k];
//      }
//      nd = 0;
//      xyz1.clear();
//      xyz2.clear();
//      grd1.clear();
//      grd2.clear();
//      m1 = monomers_[3*first_index_[dimers_[i]]];
//      m2 = monomers_[3*first_index_[dimers_[i + 1]]];
//    }
//
//    // Push the coordinates
//    for (size_t j = 0; j < nat_[3*first_index_[dimers_[i]]]; j++) {
//      grd1.push_back(0.0);
//      xyz1.push_back(xyz_[3*first_index_[dimers_[i]] + j]);
//    }
//    for (size_t j = 0; j < nat_[3*first_index_[dimers_[i+1]]]; j++) {
//      grd2.push_back(0.0);
//      xyz2.push_back(xyz_[3*first_index_[dimers_[i+1]] + j]);
//    }
//    
//    nd++;
//  }
//
//  // Last Iteration is not done
//  e2b += get_2b_energy(m1, m2, nd, xyz1, xyz2, grd1, grd2);
//  nd = 0;
//  xyz1.clear();
//  xyz2.clear();
//  grd1.clear();
//  grd2.clear();
//
//  energy_ += e2b;
//
//  // 3B ENERGY
//  double e3b = 0;
//  // trimers are ordered
//  // TODO put this in chunk so can be vectorized
//  // TODO Maybe initialize here all possible trimer structs?
//  for (size_t i = 0; i < trimers_.size(); i+=3) {
//    if (monomers_[trimers_[i]] == "h2o" and
//        monomers_[trimers_[i + 1]] == "h2o" and
//        monomers_[trimers_[i + 2]] == "h2o") {
//      double grdx[27];
//      std::fill(grdx, grdx + 27, 0.0);
//      x2o::x3b_v2x pot;
//      e3b += pot.eval(xyz_.data() + 3*first_index_[trimers_[i]],
//                      xyz_.data() + 3*first_index_[trimers_[i + 1]],
//                      xyz_.data() + 3*first_index_[trimers_[i + 2]], 
//                      grdx, grdx + 9, grdx + 18);
//      for (size_t j = 0; j < 9; j++) {
//        grd_[3*first_index_[trimers_[i]] + j] += grdx[j];
//        grd_[3*first_index_[trimers_[i + 1]] + j] += grdx[j + 9];
//        grd_[3*first_index_[trimers_[i + 2]] + j] += grdx[j + 18];
//      }
//    }
//  }
//
//  energy_ += e3b;
//
//
//  // Putting gradients back to argument. All grads will be put there
//  // Including Virtual Electrostatic sites
//  // TODO Maybe change this
//  grd.clear();
//  grd = grd_;
//
//  return energy_;
//} 
////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////






