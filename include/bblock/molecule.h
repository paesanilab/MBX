#ifndef MOLECULE_H
#define MOLECULE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>

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
    
    size_t get_n_mon();
    void set_n_mon(size_t n);
    void add_monomer(std::string mon_name, double * xyz,
          std::vector<std::string> names);
//    monomer * get_monomer(size_t n);
//    monomer * get_monomer();

  private:
    size_t n_mon;
    std::vector<std::shared_ptr<monomer> > monomers;
    
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MOLECULE_H
