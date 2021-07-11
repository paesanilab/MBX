#ifndef RMSD_TOOLS_H
#define RMSD_TOOLS_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include "rmsd.h"

typedef std::map<std::string, double> mass_map_type;

namespace rmsd_tools {

std::vector<double> GetMasses(std::vector<std::string> atoms);

std::vector<std::vector<size_t> > ReadPermutations(size_t num_ats, std::string perm_file);

void GetRelevantCoordinates(std::vector<size_t> ignored_indexes, std::vector<double> xyz, std::vector<double> &new_xyz);

void GetRelevantMassesAndAtoms(std::vector<size_t> ignored_indexes, std::vector<double> masses,
                               std::vector<std::string> atoms, std::vector<double> &new_masses,
                               std::vector<std::string> &new_atoms);

double CalculateRmsd(std::vector<double> xyz1, std::vector<double> xyz2, std::vector<std::vector<size_t> > permutations,
                     std::vector<double> mass, bool use_inversion,
                     double threshold = std::numeric_limits<double>::infinity());

}  // namespace rmsd_tools

#endif  // RMSD_TOOLS_H
