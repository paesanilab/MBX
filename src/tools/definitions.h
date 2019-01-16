#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <set>
#include <utility>
#include <cstdlib>

typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

static const double EMAX1B = 60.0;

// Ignore mutual terms in the dipoles, for debugging
#define DIRECT_ONLY 0
// A useful macro for debugging against other codes by removing Thole damping entirely
#define NO_THOLE 0

#endif
