#include "errorcodes.h"

namespace errors {

  void print_error(size_t error_code, std::string fname) {

    std::cerr << "ERROR: " << std::endl;
    if (error_code == size_t(1)) {
      std::cerr << "In function " << fname 
                << ": inconsistent size of array." << std::endl;
//    } else if (error_code == size_t(2)) {
      
    } else {
      std::cerr << "FATAL ERROR. ERROR CODE NOT KNOWN..."
                << std::endl
                << "CONTACT CODE ADMIN (mrierari@ucsd.edu)"
                << std::endl;
    }
   
    exit (EXIT_FAILURE);
  }

} // namespace errors
