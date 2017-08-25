#include "system.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {initialized_ = false;}

System::~System() {}

size_t System::GetNumMol() {return nmol_;}

std::vector<size_t> System::GetDimers() {return dimers_;}

std::vector<size_t> System::GetTrimers() {return trimers_;}

std::vector<std::string> System::GetSysAtNames() {return atoms_;}

std::vector<double> System::GetSysXyz() {return xyz_;}

std::vector<size_t> System::GetMolecule(size_t n) {return molecules_[n];}

std::string System::GetMonId(size_t n) {return monomers_[n];}

size_t System::GetMonNat(size_t n) {return nat_[n];}

size_t System::GetFirstInd(size_t n) {return first_index_[n];}

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
  AddClusters(3);
  // TODO Here should go the order and rearrengement stuff
}

void System::AddMonomerInfo() {
  // TODO Add check if empty or not initialized
  std::vector<double> xyz = xyz_;
  xyz_.clear();

  std::vector<std::string> atoms = atoms_;
  atoms_.clear();

  // TODO Improve the sorting. Later on this will do also domain decomposition
  //std::vector<std::string> monomers;
  //for (size_t i = 0; i < monomers_.size() - 1; i++) {
  //  std::string min_mon = monomers_[0];
  //  size_t index_min = 0;
  //  for (size_t j = i + 1; j < monomers_.size()) {
  //    if (monomers_[j] < min_mon) {
  //      min_mon = monomers_[j];
  //      index_min = j;
  //    }
  //  }
  //  monomers.push_back(min_mon);
  //  initial_order_.push_back(index_min);
  //}

  //monomers_.clear();
  //monomers_ = monomers;

  // At this point, the monomers are sorted by name.
  // Filling the numer of sites and atoms, chg, pols and polfacs
  // TODO maybe use a swich would be better than if else
  // TODO maybe we should try to find out number of sites first and reserve
  // memory, rather than pushing back?
  size_t count = 0;
  for (size_t i = 0; i < monomers_.size(); i++) {
    if (monomers_[i] == "h2o") {
      // Filling things for water.
      // TODO: WARNING. For water, charges, and position of the M-site
      // depends on the 3 atom coordinate.  

      // Site Info
      sites_.push_back(4);
      nat_.push_back(3);
      first_index_.push_back(count);
      count += sites_[i];
      
      // Charge info
      chg_.push_back(0.0);
      chg_.push_back(0.0);
      chg_.push_back(0.0);
      chg_.push_back(0.0);

      // Pol info
      pol_.push_back(0.0);
      pol_.push_back(0.0);
      pol_.push_back(0.0);
      pol_.push_back(0.0);

      // Polfac info
      polfac_.push_back(0.0);
      polfac_.push_back(0.0);
      polfac_.push_back(0.0);
      polfac_.push_back(0.0);
    } else {
      //std::cerr << "No data in the dataset for monomer: "
      //          << monomers_[i] << std::endl;
      // Exit program
    }
  }
  
  // Setting total number of sites
  nsites_ = count;

  // Rearranging coordinates to account for virt sites
  xyz_.reserve(3*nsites_);
  count = 0;
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

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////






