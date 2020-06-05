#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <string>

namespace kit { namespace io {

int to_int(const std::string&);
size_t to_uint(const std::string&);
double to_double(const std::string&);

}} // namespace kit::io

#endif // IO_UTILS_H
