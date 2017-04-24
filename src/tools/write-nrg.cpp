#include "write-nrg.h" 

namespace tools {

void write_nrg(const char * filename, std::vector<bblock::system> systems ) {
  
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
      std::shared_ptr<bblock::molecule> m = systems[i].get_molecule(j);
      for (size_t k = 0; k < m->get_n_mon(); k++) {
        std::shared_ptr<bblock::monomer> mon = m->get_monomer(k);
        std::string mon_name = mon->get_monomer_id();
        os << "MONOMER " << mon_name << std::endl;
        size_t ns = mon->get_n_realsites();
        double xyz[3*ns];
        std::vector<std::string> an = mon->get_atom_names();
        mon->get_real_xyz(xyz);
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
