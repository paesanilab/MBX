#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <iostream>
#include <stdlib.h>

namespace errors {

  void print_error(size_t error_code, std::string fname);

} // namespace errors

#endif // ERRORCODES_H
