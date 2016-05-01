#include "write-nrg.h" 

namespace tools {

void write_nrg(char * filename, std::vector<bblock::system> systems ) {
  
  assert(filename);

  std::ofstream ofs(filename);
  if (!ofs)
    throw std::runtime_error("could not open NRG file for writting");
  
  return;
}

void save_nrg(std::ostream & os, std::vector<bblock::system> systems ) {
  for (int i = 0; i < systems.size(); i++) {
    os << "SYSTEM " << i << std::endl;
    for (int j = 0; j < systems[i].molecules.size(); j++) {
      // TODO: Put here the connectivity after molecule
      os << "MOLECULE " << i << "." << j << std::endl;
      for (int k = 0; k < systems[i].molecules[j].size(); k++) {
        os << "MONOMER " << std::endl;
        // TODO: Finish properly the writting function
        os << "ENDMON " << std::endl;
      }
      os << "ENDMOL " << std::endl;
    } 
    os << "ENDSYS " << std::endl;
  }
}
