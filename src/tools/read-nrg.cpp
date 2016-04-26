#include "read-nrg.h" 

namespace tools {

void read_nrg(char * filename, std::vector<bblock::system> & systems ) {
  
  assert(filename);

  std::ifstream ifs(filename);
  if (!ifs)
    throw std::runtime_error("could not open NRG file for reading");
  
  size_t lineno(0);
  size_t sysno(0);

  while (!ifs.eof()) {
    bblock::system sys;
    read_system(lineno, ifs, sys);

//    if (sys.get_n_mol() > 0) {
      systems.push_back(sys);
//    }
  }

  return;
}

void read_foo(char * filename, double * systems ) {
  
  assert(filename);
  assert(systems);

  return;
}

void read_system(size_t& lineno, std::istream& ifs, bblock::system& sys) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);
  lineno++;

  std::string word;
  std::istringstream iss(line);
  iss >> word;
  
  if (iss.fail()) {
    std::ostringstream oss;
    oss << "unexpected text at line " << lineno
        << " of the NRG file:" << std::endl << iss.str() << std::endl;
    throw std::runtime_error(oss.str());
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);

  if (word != "system") {
    std::ostringstream oss;
    oss << "No SYSTEM in line  " << lineno
        << " of the NRG file:" << std::endl << iss.str() << std::endl; 
    throw std::runtime_error(oss.str());
  }

  size_t molno(0),molno_read(0);

  while (word != "endsys") {
    bblock::molecule molec;
    read_molecule(lineno, ifs, molec);

//    if (molec.get_n_mon() > 0) {
//      sys.add_molecule(molec);
//      molno++;
//    }

    iss.clear();
    std::getline(ifs, line);
    lineno++;
    iss.str(line);
    iss >> word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  }

  
  return;
}

void read_molecule(size_t& lineno, std::istream& ifs, bblock::molecule& molec) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);
  lineno++;

  std::string word;
  std::istringstream iss(line);
  iss >> word;
  
  if (iss.fail()) {
    std::ostringstream oss;
    oss << "unexpected text at line " << lineno
        << " of the NRG file:" << std::endl << iss.str() << std::endl;
    throw std::runtime_error(oss.str());
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word != "molecule") {
    std::ostringstream oss;
    oss << "No MOLECULE in line  " << lineno
        << " of the NRG file:" << std::endl;
    throw std::runtime_error(oss.str());
  }
  
  read_monomers(lineno, ifs, molec);


  return;

}

void read_monomers(size_t& lineno, std::istream& ifs, bblock::molecule& molec) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);
  lineno++;

  std::string word;
  std::istringstream iss(line);
  iss >> word;

  if (iss.fail()) {
    std::ostringstream oss;
    oss << "unexpected text at line " << lineno
        << " of the NRG file:" << std::endl << iss.str() << std::endl;
    throw std::runtime_error(oss.str());
  }

  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word != "monomer") {
    std::ostringstream oss;
    oss << "No MONOMER in line  " << lineno
        << " of the NRG file:" << std::endl;
    throw std::runtime_error(oss.str());
  }

  std::string mon_name;
  iss.clear();
  iss.str(line);
  iss >> word >> mon_name;
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);

  while (word != "endmol") {
  
    if (word != "monomer" || mon_name.empty() ) {
      std::ostringstream oss;
      oss << "No MONOMER in line  " << lineno
          << " of the NRG file:" << std::endl << iss.str() << std::endl;
      throw std::runtime_error(oss.str());
    }
    
    std::transform(mon_name.begin(), mon_name.end(), mon_name.begin(), ::tolower);

    std::vector<std::string> names;
    double xyz[1000];
    size_t i(0);
    
    std::getline(ifs, line);
    lineno++;
    iss.clear();
    iss.str(line);
 
    while (word != "endmon") {

      std::string name;
      double x,y,z;
      
      iss >> name >> x >> y >> z;
      names.push_back(name);
      xyz[i++] = x;
      xyz[i++] = y;
      xyz[i++] = z;

      std::getline(ifs, line);
      lineno++;
      iss.clear();
      iss.str(line);

      iss >> word;
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      iss.clear();
      iss.str(line);
    }

    molec.add_monomer(mon_name, xyz, names);

    iss.clear();
    std::getline(ifs, line);
    lineno++;
    iss.str(line);
    iss >> word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  }
  
  return;
}



} // namespace tools

















