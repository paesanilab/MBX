#include "system.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

System::System() {
  initialized_ = false;
}

System::~System() {
}

size_t System::GetNumMol() {
  return nmol_;
}

std::vector<size_t> System::GetMolecule(size_t n) {
  return molecules_[n];
}

std::string System::GetMonId(size_t n) {
  return monomers_[n];
}

size_t System::GetMonNat(size_t n) {
  return nat_[n];
}

size_t System::GetFirstInd(size_t n) {
  return first_index_[n];
}

std::vector<std::string> System::GetSysAtNames() {
  return atoms_;
}

std::vector<double> System::GetSysXyz() {
  return xyz_;
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

  AddMonomerInfo();
  
  nmol_ = molecules_.size();
  // TODO Here should go the order and rearrengement stuff
}

void System::AddMonomerInfo() {
  // TODO Add check if empty or not initialized
  std::vector<double> xyz = xyz_;
  xyz_.clear();

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
    } else if (monomers_[i] == "co2") {
      // Site Info
      sites_.push_back(3);
      nat_.push_back(3);
      first_index_.push_back(count);
      count += sites_[i];

      // Charge info
      chg_.push_back(0.454467);
      chg_.push_back(-0.227224);
      chg_.push_back(-0.227224);

      // Pol info
      pol_.push_back(1.43039);
      pol_.push_back(0.771519);
      pol_.push_back(0.771519);

      // Polfac info
      polfac_.push_back(1.43039);
      polfac_.push_back(0.771519);
      polfac_.push_back(0.771519);
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
  for (size_t i = 0; i < monomers_.size(); i++) {
    //size_t k = initial_order_[i];
    std::copy(xyz.begin() + 3 * count,
              xyz.begin() + 3 * (nat_[i] + count),
              xyz_.begin() + 3 * first_index_[i]);
  }
  

}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////
