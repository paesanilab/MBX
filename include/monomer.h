#ifndef MONOMER_H
#define MONOMER_H

//typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: monomer

////////////////////////////////////////////////////////////////////////////////

class monomer {

  public:
    monomer();
    ~monomer();
    void clear();
    
    int get_n_sites();

//    double * get_xyz();
//    double * get_charge();
//    double * get_polarizabilityi();
//    double * get_polfac();

//    char * get_id();

  private:
    int n_sites;

//    double * xyz;
//    double * charge;
//    double * polarizability;
//    double * polfac;

//    char * id;

//    excluded_set_type excluded12;
//    excluded_set_type excluded13;
//    excluded_set_type excluded14;

    
    
        
    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MONOMER_H
