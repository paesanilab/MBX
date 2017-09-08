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
  xyz_.reserve(3*nsites_);
  size_t count = 0;
  for (size_t i = 0; i < 3*nsites_; i++) 
    xyz_.push_back(0.0);
  for (size_t i = 0; i < nsites_; i++)
    atoms_.push_back("virt");
    
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
  grd_.reserve(3*nsites_);
  for (size_t i = 0; i < 3*nsites_; i++)
    grd_.push_back(0.0);
  

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

double System::Energy() {

  // 1B ENERGY
  // Loop overall the monomers and get their energy
  size_t cmt = 0;
  size_t ccrd = 0;
  energy_ = 0.0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    // Useful variables
    std::vector<double> energy;
    size_t ncoord = 3 * nat_[cmt] * mon_type_count_[k].second;
    // XYZ with real sites
    double xyz[ncoord];
    
    // Set up real coordinates
    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
      std::copy(xyz_.begin() + ccrd + 3 * i * sites_[cmt],
                xyz_.begin() + ccrd + 3 * (i+1) * sites_[cmt],
                xyz + 3 * i * nat_[cmt]);
    }

    // Get energy of the chunk as function of monomer
    if (mon_type_count_[k].first == "h2o") {
      energy = ps::pot_nasa(xyz, 0, mon_type_count_[k].second);
    } else {
      std::fill(energy.begin(), energy.end(),0.0);
    }

    // Add energy to energy_
    for (size_t i = 0; i < mon_type_count_[k].second; i++) 
      energy_ += energy[i];

    // Update ccrd and cmt
    ccrd += 3 * mon_type_count_[k].second * sites_[cmt];
    cmt += mon_type_count_[k].second;
  }

  // 2B ENERGY

  // 3B ENERGY

  return energy_;
}

double System::Energy(double * grd) {
  // 1B ENERGY
  // Loop overall the monomers and get their energy
  size_t cmt = 0;
  size_t ccrd = 0;
  energy_ = 0.0;
  for (size_t k = 0; k < mon_type_count_.size(); k++) {
    // Useful variables
    std::vector<double> energy;
    size_t ncoord = 3 * nat_[cmt] * mon_type_count_[k].second;
    // XYZ with real sites
    double xyz[ncoord];
    
    // Set up real coordinates
    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
      std::copy(xyz_.begin() + ccrd + 3 * i * sites_[cmt],
                xyz_.begin() + ccrd + 3 * (i+1) * sites_[cmt],
                xyz + 3 * i * nat_[cmt]);
    }

    // Get energy of the chunk as function of monomer
    double grd2[ncoord];
    std::fill(grd2, grd2 + ncoord, 0.0);
    if (mon_type_count_[k].first == "h2o") {
      energy = ps::pot_nasa(xyz, grd2, mon_type_count_[k].second);
    } else {
      std::fill(energy.begin(), energy.end(),0.0);
    }

    // Add energy to energy_
    for (size_t i = 0; i < mon_type_count_[k].second; i++) {
      energy_ += energy[i];

      // Reorganize gradients
      for (size_t j = 0; j < 3*nat_[cmt]; j++) {
        grd_[ccrd + i*sites_[cmt] + j] += grd2[i*nat_[cmt] + j];
      }
    }
    
    // Update ccrd and cmt
    ccrd += 3 * mon_type_count_[k].second * sites_[cmt];
    cmt += mon_type_count_[k].second;
  }

  // 2B ENERGY

  // 3B ENERGY


  // Putting gradients back to argument. All grads will be put there
  for (size_t i = 0; i < nmon_; i++) {
    std::copy(grd_.begin(), grd_.end(), grd); 
  }

  return energy_;
} 
////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////






