#ifndef MAIN_PARSE_ARGS_H
#define MAIN_PARSE_ARGS_H

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

// Helpers to parse numeric command-line arguments. On invalid input
// (non-numeric, trailing characters, or out of range) they print an error
// naming the offending argument and exit with EXIT_FAILURE.

inline int ParseIntArg(const std::string &arg, const std::string &name) {
    try {
        size_t pos = 0;
        int value = std::stoi(arg, &pos);
        if (pos != arg.size()) throw std::invalid_argument(arg);
        return value;
    } catch (const std::exception &) {
        std::cerr << "Invalid value for " << name << ": '" << arg << "' (expected integer value)" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

inline double ParseDoubleArg(const std::string &arg, const std::string &name) {
    try {
        size_t pos = 0;
        double value = std::stod(arg, &pos);
        if (pos != arg.size()) throw std::invalid_argument(arg);
        return value;
    } catch (const std::exception &) {
        std::cerr << "Invalid value for " << name << ": '" << arg << "' (expected floating-point value)" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#endif  // MAIN_PARSE_ARGS_H
