#include "monomer.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////

Monomer::Monomer() {
}

Monomer::~Monomer() {
}


size_t Monomer::GetNumSites(){
  return n_sites;
}

size_t Monomer::GetNumRealSites(){
  return n_real_sites;
}

std::string Monomer::GetMonomerId(){
  return mon_id;
}

void Monomer::GetRealSitesXyz(double * coords) {

  // Set coordinates
  std::copy(xyz.get(), xyz.get() + 3*n_real_sites, coords);
}

std::vector<std::string> Monomer::GetAtomNames() {
  return at_names;
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////
