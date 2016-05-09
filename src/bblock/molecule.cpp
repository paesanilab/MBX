#include "molecule.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

molecule::molecule() {
  n_mon=0;
}

molecule::~molecule() {
}

void molecule::add_monomer(std::string mon_name, double * xyz, std::vector<std::string> names){
  if (mon_name == "h2o") {
    bblock::h2o * mon = new bblock::h2o(xyz, names);
    monomers.push_back(mon);
  } else {
    std::ostringstream oss;
    oss << "Monomer " << mon_name
        << " not found in monomer library." << std::endl;
    throw std::runtime_error(oss.str());
  }
  
  return;
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: System

////////////////////////////////////////////////////////////////////////////////
