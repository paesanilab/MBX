/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#ifndef UNITTESTS_SETUP_LINKER_H
#define UNITTESTS_SETUP_LINKER_H

#define SETUP_LINKER                                                                                                   \
    std::vector<std::string> atom_names = {"N", "N", "C", "C", "C", "H", "H", "O", "C", "H", "H"};                     \
    const size_t n_atoms = 11;                                                                                         \
    const size_t n_sites = 11;                                                                                         \
    std::vector<double> coords{-0.81297600, -1.43297200, 1.18013800, -1.91222900, -2.45600500, 2.76442800,             \
                               -1.64346800, -1.26881000, 2.20574800, -2.20839000, -0.00003800, 2.70514200,             \
                               -1.23969900, -3.42189400, 2.08801200, -1.29576200, -4.46487800, 2.34890100,             \
                               -1.90466600, 0.90874300,  2.15230300, -2.94449800, 0.02836900,  3.64629400,             \
                               -0.55467300, -2.77573100, 1.09960900, 0.10477500,  -3.19431400, 0.35867900,             \
                               -0.47113843, -0.67416455, 0.60543087};                                                  \
    std::string bon_func_form = "harm";                                                                                \
    std::string ang_func_form = "harm";                                                                                \
    std::string dih_func_form = "cos";                                                                                 \
    std::vector<std::vector<size_t> > bon_idxs = {{1, 3}, {2, 3}, {1, 9}, {2, 5},  {1, 11}, {3, 4},                    \
                                                  {4, 7}, {4, 8}, {5, 6}, {9, 10}, {5, 9}};                            \
    std::vector<std::vector<size_t> > ang_idxs = {{1, 3, 2},  {4, 3, 1},  {4, 3, 2}, {9, 5, 2},  {5, 9, 1}, {6, 5, 2}, \
                                                  {10, 9, 1}, {3, 1, 9},  {3, 2, 5}, {3, 1, 11}, {3, 4, 7}, {3, 4, 8}, \
                                                  {9, 5, 6},  {5, 9, 10}, {7, 4, 8}, {9, 1, 11}};                      \
    std::vector<std::vector<size_t> > dih_idxs = {                                                                     \
        {7, 4, 3, 1},  {7, 4, 3, 2},   {8, 4, 3, 1}, {8, 4, 3, 2},  {1, 9, 5, 2},  {6, 5, 9, 1},                       \
        {10, 9, 5, 2}, {9, 5, 2, 3},   {5, 9, 1, 3}, {6, 5, 2, 3},  {10, 9, 1, 3}, {2, 3, 1, 9},                       \
        {1, 3, 2, 5},  {4, 3, 1, 9},   {4, 3, 2, 5}, {6, 5, 9, 10}, {2, 3, 1, 11}, {4, 3, 1, 11},                      \
        {5, 9, 1, 11}, {10, 9, 1, 11}, {3, 4, 1, 2}, {4, 3, 7, 8},  {5, 9, 6, 2},  {9, 5, 10, 1}};                     \
    std::vector<std::vector<double> > l_bonds = {{882.200000}, {882.200000}, {1050.800000}, {1050.800000},             \
                                                 {771.200000}, {742.000000}, {621.400000},  {1275.400000},             \
                                                 {704.000000}, {704.000000}, {839.600000}};                            \
    std::vector<std::vector<double> > nl_bonds = {{1.369000}, {1.369000}, {1.317000}, {1.317000},                      \
                                                  {1.023000}, {1.468000}, {1.112000}, {1.218000},                      \
                                                  {1.082000}, {1.082000}, {1.428000}};                                 \
    std::vector<std::vector<double> > l_angles = {{139.600000}, {132.400000}, {132.400000}, {143.200000},              \
                                                  {143.200000}, {104.200000}, {104.200000}, {143.600000},              \
                                                  {143.600000}, {101.800000}, {94.200000},  {138.200000},              \
                                                  {91.800000},  {91.800000},  {108.200000}, {101.800000}};             \
    for (size_t i = 0; i < l_angles.size(); i++) {                                                                     \
        l_angles[i][0] = l_angles[i][0];                                                                               \
    }                                                                                                                  \
    std::vector<std::vector<double> > nl_angles = {{125.700000}, {123.300000}, {123.300000}, {112.600000},             \
                                                   {112.600000}, {118.500000}, {118.500000}, {105.500000},             \
                                                   {105.500000}, {116.100000}, {114.800000}, {123.900000},             \
                                                   {128.000000}, {128.000000}, {121.900000}, {116.100000}};            \
    for (size_t i = 0; i < nl_angles.size(); i++) {                                                                    \
        nl_angles[i][0] = nl_angles[i][0] / 180.0 * pi;                                                                \
    }                                                                                                                  \
    std::vector<std::vector<double> > l_dhs = {                                                                        \
        {2.875000}, {2.875000}, {2.875000}, {2.875000}, {4.000000}, {4.000000},  {4.000000}, {4.750000},               \
        {4.750000}, {4.750000}, {4.750000}, {4.750000}, {4.750000}, {4.750000},  {4.750000}, {4.000000},               \
        {4.750000}, {4.750000}, {4.750000}, {4.750000}, {1.100000}, {10.500000}, {1.100000}, {1.100000}};              \
    std::vector<std::vector<double> > nl_dhs = {                                                                       \
        {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359},            \
        {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359},            \
        {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359},            \
        {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359},            \
        {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}, {2, 3.14159265359}};

#endif
