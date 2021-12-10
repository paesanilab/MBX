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

#ifndef UNITTESTS_SETUP_H2O_5_BR_1_H
#define UNITTESTS_SETUP_H2O_5_BR_1_H

#define SETUP_H2O_5_BR_1                                                                                              \
    double one_body_energy = 1.0759450217e+00;                                                                        \
    double two_body_energy = 2.5636777719e+01;                                                                        \
    double three_body_energy = -3.2745042811e-01;                                                                     \
    double dispersion_energy = -1.5309349723e+01;                                                                     \
    double electrostatic_energy = -7.3208264066e+01;                                                                  \
    double total_energy = -6.2132341476e+01;                                                                          \
    const int n_monomers = 6;                                                                                         \
    std::vector<double> coords{                                                                                       \
        1.3217649840e+00,  -9.7410428510e-02, -1.7666034010e-01, 1.4174979920e+00,  6.3858453390e-01,                 \
        4.4398687760e-01,  3.5406637980e-01,  -1.5881187310e-01, -2.4560734510e-01, 1.1357281236e+00,                 \
        4.6516419793e-02,  -5.8953201320e-02, -2.2081312000e+00, 1.9766936770e-01,  2.3930482270e-01,                 \
        2.9449221580e-01,  -2.3515357490e+00, 1.4448241600e+00,  9.3123346580e-01,  -1.7883803440e+00,                \
        9.8602249620e-01,  -4.7767612580e-01, -1.7721721880e+00, 1.4851854640e+00,  2.6559837980e-01,                 \
        -2.1077753962e+00, 1.3555484694e+00,  -1.1228350600e+00, -3.3355538770e+00, -9.7989539360e-01,                \
        -5.7480679740e-01, -3.2522363550e+00, -1.8933695080e-01, -1.5382865730e+00, -2.4675748990e+00,                \
        -1.0199580330e+00, -1.0945493543e+00, -3.1325914953e+00, -8.1977453149e-01, 1.2756361190e-01,                 \
        2.3112777530e+00,  -1.6555462120e+00, 7.5744314660e-01,  1.5825624740e+00,  -1.6347682740e+00,                \
        -6.8646837650e-01, 1.8720990840e+00,  -1.3681834210e+00, 8.8274052235e-02,  2.0621035605e+00,                 \
        -1.5898033272e+00, 5.9987029440e-01,  2.3606804510e+00,  1.2735125440e+00,  -2.1307295330e-01,                \
        1.8440595680e+00,  1.3669355470e+00,  5.1046011720e-01,  2.6935426750e+00,  3.7069924640e-01,                 \
        4.0735008620e-01,  2.3214749088e+00,  1.1008263395e+00};                                                      \
    std::vector<double> real_coords{                                                                                  \
        1.321764984e+00,  -9.741042851e-02, -1.766603401e-01, 1.417497992e+00,  6.385845339e-01,  4.439868776e-01,    \
        3.540663798e-01,  -1.588118731e-01, -2.456073451e-01, -2.208131200e+00, 1.976693677e-01,  2.393048227e-01,    \
        2.944922158e-01,  -2.351535749e+00, 1.444824160e+00,  9.312334658e-01,  -1.788380344e+00, 9.860224962e-01,    \
        -4.776761258e-01, -1.772172188e+00, 1.485185464e+00,  -1.122835060e+00, -3.335553877e+00, -9.798953936e-01,   \
        -5.748067974e-01, -3.252236355e+00, -1.893369508e-01, -1.538286573e+00, -2.467574899e+00, -1.019958033e+00,   \
        1.275636119e-01,  2.311277753e+00,  -1.655546212e+00, 7.574431466e-01,  1.582562474e+00,  -1.634768274e+00,   \
        -6.864683765e-01, 1.872099084e+00,  -1.368183421e+00, 5.998702944e-01,  2.360680451e+00,  1.273512544e+00,    \
        -2.130729533e-01, 1.844059568e+00,  1.366935547e+00,  5.104601172e-01,  2.693542675e+00,  3.706992464e-01};   \
    std::vector<std::string> atom_names{"O", "H", "H", "Br", "O", "H", "H", "O",                                      \
                                        "H", "H", "O", "H",  "H", "O", "H", "H"};                                     \
    std::vector<std::string> monomer_names = {"h2o", "br-", "h2o", "h2o", "h2o", "h2o"};                               \
    std::vector<size_t> n_atoms_vector = {3, 1, 3, 3, 3, 3};                                                          \
    std::vector<size_t> n_sites_vector = {4, 1, 4, 4, 4, 4};                                                          \
    std::vector<size_t> first_index = {0, 4, 5, 9, 13, 17};                                                           \
    std::vector<size_t> first_index_realSites = {0, 3, 4, 7, 10, 13};                                                 \
    std::vector<size_t> islocal = {1, 1, 1, 1, 1, 1};                                                                 \
    const size_t n_atoms = 16;                                                                                        \
    const size_t n_sites = 21;                                                                                        \
    std::vector<double> polfac{                                                                                       \
        1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 7.1668000000e+00, 1.3100000000e+00,   \
        2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,   \
        1.3100000000e+00, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 1.3100000000e+00,   \
        2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};                                                        \
    std::vector<double> pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, 7.1668000000e+00, \
                            1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, 1.3100000000e+00, \
                            2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, 1.3100000000e+00, 2.9400000000e-01, \
                            2.9400000000e-01, 0.0000000000e+00, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, \
                            0.0000000000e+00};                                                                        \
    std::vector<double> charges{0.0000000000e+00,  5.6583693130e-01, 5.6516604109e-01, -1.1310029724e+00,             \
                                -1.0000000000e+00, 0.0000000000e+00, 5.6770589454e-01, 5.6767184419e-01,              \
                                -1.1353777387e+00, 0.0000000000e+00, 5.6873442599e-01, 5.6908414806e-01,              \
                                -1.1378185740e+00, 0.0000000000e+00, 5.6842486717e-01, 5.6768445681e-01,              \
                                -1.1361093240e+00, 0.0000000000e+00, 5.6708114045e-01, 5.6727943429e-01,              \
                                -1.1343605747e+00};                                                                   \
    std::vector<double> real_polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 7.1668000000e+00,           \
                                    1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,           \
                                    2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01,           \
                                    2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};          \
    std::vector<double> real_pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 7.1668000000e+00,              \
                                 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,              \
                                 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01,              \
                                 2.9400000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};             \
    std::vector<double> real_charges{0.0000000000e+00, 5.6583693130e-01, 5.6516604109e-01, -1.0000000000e+00,         \
                                     0.0000000000e+00, 5.6770589454e-01, 5.6767184419e-01, 0.0000000000e+00,          \
                                     5.6873442599e-01, 5.6908414806e-01, 0.0000000000e+00, 5.6842486717e-01,          \
                                     5.6768445681e-01, 0.0000000000e+00, 5.6708114045e-01, 5.6727943429e-01};         \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"br-", 1}, {"h2o", 5}};                       \
    std::vector<size_t> internal_original_to_current_order{1, 0, 2, 3, 4, 5};                                         \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{1, 4}, {0, 0}, {2, 5}, {3, 9}, {4, 13}, {5, 17}};  \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{1, 3}, {0, 0},  {2, 4},                 \
                                                                             {3, 7}, {4, 10}, {5, 13}};               \
    std::vector<std::string> internal_monomer_names{"br-", "h2o", "h2o", "h2o", "h2o", "h2o"};
#endif
