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

#ifndef UNITTESTS_SETUP_H2O_2_VIRIAL_H
#define UNITTESTS_SETUP_H2O_2_VIRIAL_H

#define SETUP_H2O_2_VIRIAL                                                                          \
    const size_t n_monomers = 2;                                                                                    \
    std::vector<std::string> atom_names{"O", "H", "H", "O", "H", "H"};                                                           \
    std::vector<std::string> monomer_names = {"h2o","h2o"};                                                             \
    std::vector<size_t> n_atoms_vector = {3,3};                                                                       \
    std::vector<size_t> n_sites_vector = {4,4};                                                                       \
    std::vector<size_t> first_index = {0,4};                                                                          \
    std::vector<size_t> first_index_realSites = {0,3};                                                                \
    const size_t n_atoms = 6;                                                                                          \
    const size_t n_sites = 8;                                                                                          \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                \
    std::vector<double> virial_1b = {   \
        -21.11278681,  21.40377629,  -5.88668738, \
         21.40377629,  -3.20344168,   1.01648662, \
        -5.88668738 ,   1.01648662,  -3.40683556}; \
    std::vector<double> virial_2b = {14.17836998,  1.04493547,  0.4985588 , \
  1.04493547,        0.09813098, -0.59318356,  \
 0.4985588       ,  -0.59318356        ,  0.19999283}; \
    std::vector<double> real_coords = std::vector<double>{                                          \
                 -1.58972425,    1.04337922,   -0.08780840,                                         \
                 -0.63591971,    0.97898520,    0.00000000,                                         \
                 -1.90066280,    1.74501050,   -0.66454990,                                         \
                  1.64924507,    1.08594656,    0.00000000,                                         \
                  2.60878026,    1.09587704,   -0.02817115,                                         \
                  1.33830653,    1.78757784,    0.57674150};                                        
#endif
