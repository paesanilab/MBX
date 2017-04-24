#include "molecule.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Molecule

////////////////////////////////////////////////////////////////////////////////

molecule::molecule() {
  n_mon=0;
}

molecule::~molecule() {
}

void molecule::add_monomer(std::string mon_name, double * xyz, std::vector<std::string> names){
  // Convert monomer name to lower case to avoid conflicts
  std::transform(mon_name.begin(), mon_name.end(), mon_name.begin(), ::tolower);

  // Add corresponding monomer
  if (mon_name == "h2o") {
    std::shared_ptr<bblock::monomer>  mon = 
         std::shared_ptr<bblock::h2o> (new bblock::h2o(xyz, names));
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

size_t molecule::get_n_mon() {
  return n_mon;
}

void molecule::set_n_mon(size_t n) {
  n_mon = n;
}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Molecule

////////////////////////////////////////////////////////////////////////////////
