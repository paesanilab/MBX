#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>

#include "molecule.h"

//Debugging
#include <iostream>


////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: System

////////////////////////////////////////////////////////////////////////////////

class system {

  public:
    system();
    ~system();
    void clear();
    
    size_t get_n_mol();
    void set_n_mol(size_t n);
    std::shared_ptr<molecule> get_molecule(size_t n);
//    molecule  get_molecule();
    
    void add_molecule(std::shared_ptr<bblock::molecule> molec);

  private:
    size_t n_mol;
    std::vector<std::shared_ptr<molecule>> molecules;
    
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // SYSTEM_H
