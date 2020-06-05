#include <sstream>
#include <stdexcept>

#include "io-utils.h"

////////////////////////////////////////////////////////////////////////////////

namespace kit { namespace io {

//----------------------------------------------------------------------------//

int to_int(const std::string& s)
{
    int res;

    std::istringstream iss(s);
    iss >> res;

    if (iss.fail() || !iss.eof()) {
        std::ostringstream oss;
        oss << "could not convert '" << s << "' to integer";
        throw std::runtime_error(oss.str());
    }

    return res;
}

//----------------------------------------------------------------------------//

size_t to_uint(const std::string& s)
{
    int res;

    res = to_int(s);

    if (res < 0) {
        std::ostringstream oss;
        oss << "could not convert '" << s << "' to positive integer";
        throw std::runtime_error(oss.str());
    }

    return size_t(res);
}

//----------------------------------------------------------------------------//

double to_double(const std::string& s)
{
    double res;

    std::istringstream iss(s);
    iss >> res;

    if (iss.fail() || !iss.eof()) {
        std::ostringstream oss;
        oss << "could not convert '" << s << "' to double";
        throw std::runtime_error(oss.str());
    }

    return res;
}

//----------------------------------------------------------------------------//

}} // namespace kit::io

////////////////////////////////////////////////////////////////////////////////
