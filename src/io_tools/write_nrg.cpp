#include "write_nrg.h" 

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
    // Getting system props
    std::vector<double> xyz = systems[i].GetSysXyz();
    std::vector<std::string> an = systems[i].GetSysAtNames();
    os << "SYSTEM " << i << std::endl;
    for (size_t j = 0; j < systems[i].GetNumMol(); j++) {
      os << "MOLECULE " << i << "." << j << std::endl;
      std::vector<size_t> m = systems[i].GetMolecule(j);
      for (size_t k = 0; k < m.size(); k++) {
        std::string mon_name = systems[i].GetMonId(m[k]);
        os << "MONOMER " << mon_name << std::endl;
        size_t ns = systems[i].GetMonNat(k);
        size_t first_ind = systems[i].GetFirstInd(k);
        for (size_t n = 0 ; n < ns ; n++) {
          os << std::scientific;
          os << std::setprecision(8);
          os << std::setw(5) << std::left << an[n + first_ind]
             << std::setw(20) << std::right << xyz[3*(n + first_ind)]
             << std::setw(20) << std::right << xyz[3*(n + first_ind) + 1]
             << std::setw(20) << std::right << xyz[3*(n + first_ind) + 2]
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
