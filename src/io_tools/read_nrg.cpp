#include "read_nrg.h" 

namespace tools {

void ReadNrg(char * filename, std::vector<bblock::System> & systems ) {
  assert(filename);
  std::ifstream ifs(filename);

  if (!ifs)
    throw std::runtime_error("could not open NRG file for reading");
  
  size_t lineno(0);
  size_t sysno(0);

  while (true) {
    bblock::System sys;
    ReadSystem(lineno, ifs, sys);
    
    systems.push_back(sys);
    sysno++;

    std::streampos oldpos = ifs.tellg();
    std::string line;
    std::getline(ifs, line);
    if (ifs.eof() or line.empty()) {
      break;
    } else {
      ifs.seekg(oldpos);
    }
  }
  return;
}

void ReadSystem(size_t& lineno, std::istream& ifs, bblock::System& sys) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::string line;
  std::getline(ifs, line);
  lineno++;
  
  size_t mon_count = 0;

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

  while (word != "endsys") {
    ReadMolecule(lineno, ifs, sys, mon_count);

    iss.clear();

    std::streampos oldpos = ifs.tellg();

    std::getline(ifs, line);
    lineno++;
    iss.str(line);
    iss >> word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    if (word == "molecule") {
      ifs.seekg(oldpos);
      lineno--;
    }
  }

  sys.Initialize();
  return;
}

void ReadMolecule(size_t& lineno, std::istream& ifs, 
                  bblock::System& sys, size_t& mon_count) {
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
  
  ReadMonomers(lineno, ifs, sys, mon_count);

  return;

}

void ReadMonomers(size_t& lineno, std::istream& ifs, 
                  bblock::System& sys, size_t& mon_count) {
  assert(ifs);

  if (ifs.eof())
    return;

  std::vector<size_t> molec;
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
    std::vector<double> xyz;
    
    std::getline(ifs, line);
    lineno++;
    iss.clear();
    iss.str(line);
 
    while (word != "endmon") {

      std::string name;
      double x,y,z;
      
      iss >> name >> x >> y >> z;
      names.push_back(name);
      xyz.push_back(x);
      xyz.push_back(y);
      xyz.push_back(z);

      std::getline(ifs, line);
      lineno++;
      iss.clear();
      iss.str(line);

      iss >> word;
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      iss.clear();
      iss.str(line);
      
      if (iss.eof()) {
        std::ostringstream oss;
        oss << "No ENDMON found after monomer" << mon_name
            << std::endl << iss.str() << std::endl;
        throw std::runtime_error(oss.str());
      }
 
    }

    sys.AddMonomer(xyz, names, mon_name);
    molec.push_back(mon_count);
    mon_count++;

    iss.clear();
    std::getline(ifs, line);
    lineno++;
    iss.str(line);
    iss >> word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  }

  sys.AddMolecule(molec);
  
  return;
}



} // namespace tools

















