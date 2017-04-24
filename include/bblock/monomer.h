#ifndef MONOMER_H
#define MONOMER_H

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "errorcodes.h"

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: monomer

////////////////////////////////////////////////////////////////////////////////

class monomer {

  public:
    monomer();
    virtual ~monomer();
    
    size_t get_n_sites();
    size_t get_n_realsites();
    size_t get_n_virtsites();

    std::shared_ptr<double> get_xyz();

    void set_xyz(double * coords);
    void get_real_xyz(double * coords);
    std::vector<std::string> get_atom_names();

  protected:
    size_t n_real_sites;
    size_t n_virt_sites;
    size_t n_sites;

    std::shared_ptr<double>  xyz;

    std::string mon_id;
    std::vector<std::string> at_names;
    

//    excluded_set_type excluded14;

    
    
        
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MONOMER_H
