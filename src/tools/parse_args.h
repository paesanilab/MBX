#ifndef TOOLS_PARSE_ARGS_H
#define TOOLS_PARSE_ARGS_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

// Helpers to parse numeric command-line arguments. On invalid input
// (non-numeric, trailing characters, non-finite, or out of range) they print
// an error naming the offending argument and exit with EXIT_FAILURE.

/**
 * @brief Parses an integer command-line argument, exiting on invalid input.
 *
 * Rejects non-numeric text, trailing characters, and values that do not fit
 * in an int.
 *
 * @param[in] arg Text of the command-line argument to parse (e.g. argv entry)
 * @param[in] name Name of the argument as shown in the usage message (e.g. "-o");
 * @return The parsed integer value
 */
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

/**
 * @brief Parses a floating-point command-line argument, exiting on invalid input.
 *
 * Rejects non-numeric text, trailing characters, values that do not fit in a
 * double, and non-finite values (nan and inf).
 *
 * @param[in] arg Text of the command-line argument to parse (e.g. argv entry)
 * @param[in] name Name of the argument as shown in the usage message (e.g. "<tolerance>");
 * @return The parsed double value
 */
inline double ParseDoubleArg(const std::string &arg, const std::string &name) {
    try {
        size_t pos = 0;
        double value = std::stod(arg, &pos);
        if (pos != arg.size() || !std::isfinite(value)) throw std::invalid_argument(arg);
        return value;
    } catch (const std::exception &) {
        std::cerr << "Invalid value for " << name << ": '" << arg << "' (expected finite floating-point value)"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#endif  // TOOLS_PARSE_ARGS_H
