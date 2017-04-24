#include "molecule.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Molecule

////////////////////////////////////////////////////////////////////////////////

Molecule::Molecule() {
  n_mon=0;
}

Molecule::~Molecule() {
}

void Molecule::AddMonomer(std::string mon_name, double * xyz, std::vector<std::string> names){
  // Convert monomer name to lower case to avoid conflicts
  std::transform(mon_name.begin(), mon_name.end(), mon_name.begin(), ::tolower);

  // Add corresponding monomer
  if (mon_name == "h2o") {
    std::shared_ptr<bblock::Monomer>  mon = 
         std::shared_ptr<bblock::H2O> (new bblock::H2O(xyz, names));
    monomers.push_back(mon);

  // If monomer not defined:
  } else {
    std::ostringstream oss;
    oss << "Monomer " << mon_name
        << " not found in monomer library." << std::endl;
    throw std::runtime_error(oss.str());
  }
  
  return;
}

std::shared_ptr<Monomer> Molecule::GetMonomer(size_t n) {
  return monomers[n];
}

size_t Molecule::GetNumMon() {
  return n_mon;
}

void Molecule::SetNumMon(size_t n) {
  n_mon = n;
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Molecule

////////////////////////////////////////////////////////////////////////////////
