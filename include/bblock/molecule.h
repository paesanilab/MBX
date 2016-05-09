#ifndef MOLECULE_H
#define MOLECULE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#include "monomer.h"
#include "monomers.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: molecule

////////////////////////////////////////////////////////////////////////////////

class molecule {

  public:
    molecule();
    ~molecule();
    void clear();
    
    int get_n_mon();
    void add_monomer(std::string mon_name, double * xyz,
          std::vector<std::string> names);
//    monomer * get_monomer(size_t n);
//    monomer * get_monomer();

  private:
    int n_mon;
    std::vector< monomer * > monomers;
    
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MOLECULE_H
