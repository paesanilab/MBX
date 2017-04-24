#include "monomer.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////

monomer::monomer() {
}

monomer::~monomer() {
}


size_t monomer::get_n_sites(){
  return n_sites;
}

size_t monomer::get_n_realsites(){
  return n_real_sites;
}

void monomer::get_real_xyz(double * coords) {

  // Check if coords has the proper size
  size_t ncoords = sizeof( coords );
  std::cerr << ncoords << std::endl;
  if (ncoords < n_real_sites) {
    errors::print_error(1,__func__);
  }

  // Set coordinates
  std::copy(xyz.get(), xyz.get() + 3*n_real_sites, coords);
}

std::vector<std::string> monomer::get_atom_names() {
  return at_names;
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////
