#include <stdexcept>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "read-nrg.h" 
#include "system.h"
#include "molecule.h"
#include "monomer.h"

namespace tools {

size_t read_nrg(const char * filename, std::vector<bblock::system> systems ) {
  
  assert(filename);

  std::ifstream ifs(filename);
  if (!ifs)
    throw std::runtime_error("could not open NRG file for reading");
  
  size_t lineno(0);
  size_t sysno(0);

  while (!ifs.eof()) {
    bblock::system sys;
    read_system(lineno, ifs, sys);

    if (sys.get_n_mol() > 0) {
      systems.push_back(sys);
      ++sysno
    }
  }

  return sysno;
}

size_t read_system(size_t lineno, std::istream ifs, bblock::system sys) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);

  std::string word;
  {
    istringstream iss(ifs);
    iss >> word;
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word != "system") {
    std::ostringstream oss;
    oss << "No SYSTEM in line  " << lineno
        << " of the NRG file:" << std::endl; 
    throw std::runtime_error(oss.str());
  }

  size_t molno(0),molno_read(0);

  while (word != "endsys") {
    bblock::molecule molec;
    read_molecule(lineno, ifs, molec);

    if (molec.get_n_mon() > 0) {
      sys.add_molecule(molec);
      molno++;
    }
    {
      istringstream iss(ifs);
      iss >> word;
    }
  }

  
  return molno;
}

size_t read_molecule(size_t lineno, std::istream ifs, bblock::molecule molec) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);

  std::string word;
  {
    istringstream iss(ifs);
    iss >> word;
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word != "molecule") {
    std::ostringstream oss;
    oss << "No MOLECULE in line  " << lineno
        << " of the NRG file:" << std::endl;
    throw std::runtime_error(oss.str());
  }
  
  size_t monno(0),monno_read(0);

  read_monomers(lineno, ifs, molec);

  }


  return monno;

}

size_t read_monomers(size_t lineno, std::istream ifs, bblock::molecule molec) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);

  std::string word;
  {
    istringstream iss(ifs);
    iss >> word;
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word != "monomer") {
    std::ostringstream oss;
    oss << "No MONOMER in line  " << lineno
        << " of the NRG file:" << std::endl;
    throw std::runtime_error(oss.str());
  }

  size_t siteno(0),siteno_read(0);

  while (word != "endmol") {
    std::getline(ifs, line);
  
    std::string mon_name;
    {
      istringstream iss(ifs);
      iss >> word >> mon_name;
    }
  
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (word != "monomer" || ! mon_name.empty() ) {
      std::ostringstream oss;
      oss << "No MONOMER in line  " << lineno
          << " of the NRG file:" << std::endl;
      throw std::runtime_error(oss.str());
    }
    
    std::transform(mon_name.begin(), mon_name.end(), mon_name.begin(), ::tolower);

    std::vector<std::string> names;
    double xyz[1000];
    size_t i(0);
    while (word != "endmon") {

      std::string name;
      double x,y,z;
      
      ifs >> name >> x >> y >> z;
      names.push_back(name);
      xyz[i++] = x;
      xyz[i++] = y;
      xyz[i++] = z;

      {
        istringstream iss(ifs);
        iss >> word ;
      }
    }

    molec.add_monomer(mon_name, xyz, names);

  }
  
  return monno;
}



} // namespace tools

















