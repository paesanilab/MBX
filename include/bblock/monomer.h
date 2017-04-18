#ifndef MONOMER_H
#define MONOMER_H

#include <set>
#include <string>
#include <vector>
#include <memory>

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: monomer

////////////////////////////////////////////////////////////////////////////////

class monomer {

  public:
    monomer();
    virtual ~monomer();
    
    int get_n_sites();

    std::shared_ptr<double> get_xyz();

    virtual void set_xyz(double * coords) = 0;

  protected:
    int n_real_sites;
    int n_virt_sites;
    int n_sites;

    std::shared_ptr<double>  xyz;

    std::string mon_id;
    std::vector<std::string> at_names;
    

//    excluded_set_type excluded14;

    
    
        
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MONOMER_H
