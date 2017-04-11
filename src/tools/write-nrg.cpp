#include "write-nrg.h" 

namespace tools {

void write_nrg(char * filename, std::vector<bblock::system> systems ) {
  
  assert(filename);

  std::ofstream ofs(filename);
  if (!ofs)
    throw std::runtime_error("could not open NRG file for writting");
  
  save_nrg(ofs, systems);
  return;
}

void save_nrg(std::ostream & os, std::vector<bblock::system> systems ) {
  for (size_t i = 0; i < systems.size(); i++) {
    os << "SYSTEM " << i << std::endl;
    for (size_t j = 0; j < systems[i].get_n_mol(); j++) {
      // TODO: Put here the connectivity after molecule
      os << "MOLECULE " << i << "." << j << std::endl;
      bblock::molecule * m = systems[i].get_molecule(i);
      for (size_t k = 0; k < m->get_n_mon(); k++) {
        os << "MONOMER " << std::endl;
        // TODO: Finish properly the writting function
        os << "ENDMON " << std::endl;
      }
      os << "ENDMOL " << std::endl;
    } 
    os << "ENDSYS " << std::endl;
  }
}

} // namespace tools
