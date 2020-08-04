#ifndef IO_XYZ_H
#define IO_XYZ_H

#include <vector>
#include <string>
#include <iostream>

namespace kit { namespace io {

void load_xyz(std::istream&             is,
              std::string&              comment,
              std::vector<std::string>& elements, // clears the original
              std::vector<double>&      xyz);     // clears the original

void save_xyz(std::ostream&                   os,
              const std::string&              comment,
              const std::vector<std::string>& elements,
              const std::vector<double>&      xyz);

}} // namespace kit::io

#endif // IO_XYZ_H
