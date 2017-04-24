#ifndef CU_INCLUDE_BBLOCK_MONH2O_H
#define CU_INCLUDE_BBLOCK_MONH2O_H

#include "monomer.h"


typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: h2o

////////////////////////////////////////////////////////////////////////////////

class H2O : public Monomer {

  public:
    H2O();
    H2O(double * coords, std::vector<std::string> names);
    ~H2O();
    
//  private:

    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // CU_INCLUDE_BBLOCK_MONH2O_H
