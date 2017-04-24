#include "write-nrg.h" 

namespace tools {

void WriteNrg(const char * filename, std::vector<bblock::System> systems ) {
  
  assert(filename);

  std::ofstream ofs(filename);
  if (!ofs)
    throw std::runtime_error("could not open NRG file for writting");
  
  SaveNrg(ofs, systems);
  return;
}

void SaveNrg(std::ostream & os, std::vector<bblock::System> systems ) {
  for (size_t i = 0; i < systems.size(); i++) {
    os << "SYSTEM " << i << std::endl;
    for (size_t j = 0; j < systems[i].GetNumMol(); j++) {
      // TODO: Put here the connectivity after molecule
      os << "MOLECULE " << i << "." << j << std::endl;
      std::shared_ptr<bblock::Molecule> m = systems[i].GetMolecule(j);
      for (size_t k = 0; k < m->GetNumMon(); k++) {
        std::shared_ptr<bblock::Monomer> mon = m->GetMonomer(k);
        std::string mon_name = mon->GetMonomerId();
        os << "MONOMER " << mon_name << std::endl;
        size_t ns = mon->GetNumRealSites();
        double xyz[3*ns];
        std::vector<std::string> an = mon->GetAtomNames();
        mon->GetRealSitesXyz(xyz);
        for (size_t n = 0 ; n < ns ; n++) {
          os << std::scientific;
          os << std::setprecision(8);
          os << std::setw(5) << std::left << an[n]
             << std::setw(20) << std::right << xyz[3*n]
             << std::setw(20) << std::right << xyz[3*n + 1]
             << std::setw(20) << std::right << xyz[3*n + 2]
             << std::endl;
        }
        
        os << "ENDMON " << std::endl;
      }
      os << "ENDMOL " << std::endl;
    } 
    os << "ENDSYS " << std::endl;
  }
}

} // namespace tools
