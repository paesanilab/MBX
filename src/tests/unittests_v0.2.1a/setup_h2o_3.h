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

#ifndef UNITTESTS_SETUP_H2O_3_H
#define UNITTESTS_SETUP_H2O_3_H

#define SETUP_H2O_3                                                                                        \
    const int n_monomers = 3;                                                                              \
    const int n_atoms = 9;                                                                                 \
    const int n_sites = 12;                                                                                \
    std::vector<double> chg_grad{                                                                          \
        4.2258608738e-02,  -1.2788375296e-01, -1.5779277877e-01, 6.0127744621e-02,  -4.4532284588e-02,     \
        -6.2348599069e-02, -1.0238635336e-01, 1.7241603755e-01,  2.2014137784e-01,  7.5984037989e-02,      \
        -2.8594115758e-02, -4.6125472193e-02, 2.0032834188e-01,  -7.9768681597e-03, -4.2062222845e-02,     \
        -2.7631237987e-01, 3.6570983917e-02,  8.8187695038e-02,  -1.1824264673e-01, 1.5647786872e-01,      \
        2.0391825097e-01,  -2.6045608650e-01, 5.2509152748e-02,  1.0441082191e-01,  3.7869873323e-01,      \
        -2.0898702147e-01, -3.0832907288e-01, -1.1432281854e-01, -1.6290965408e-01, 4.7572307668e-02,      \
        -6.9765097323e-02, -4.6010616134e-02, 4.2789106053e-02,  1.8408791586e-01,  2.0892027022e-01,      \
        -9.0361413721e-02, -7.7970790131e-02, -2.1557106372e-02, 5.5515918254e-02,  -1.5290858911e-01,     \
        4.7123601823e-02,  1.3190373301e-01,  2.3087937924e-01,  -2.5566495451e-02, -1.8741965127e-01,     \
        1.9229360867e-01,  1.8446676046e-01,  -1.0308822592e-01, 2.2267368643e-01,  -1.1129856899e-03,     \
        -1.7469283907e-01, -4.1496729510e-01, -1.8335377477e-01, 2.7778106499e-01,  9.6027603324e-02,      \
        9.2036867334e-02,  1.5890623459e-01,  1.2518092831e-02,  7.1962314108e-02,  6.5906678960e-02,      \
        -1.0854569616e-01, -1.6399918144e-01, -2.2481291355e-01, -1.0794108189e-02, 7.9466612187e-02,      \
        4.9824198149e-02,  -8.8056200504e-02, 1.7739275106e-01,  5.1580562794e-02,  9.8850308693e-02,      \
        -2.5685936325e-01, -1.0140476094e-01, -8.5233495135e-02, -1.7150347952e-01, -2.0873043274e-01,     \
        7.5538107672e-02,  -2.4935506517e-01, -1.1748724175e-01, 9.6953874626e-03,  4.2085854469e-01,      \
        3.2621767450e-01};                                                                                 \
    std::vector<double> polfac(n_sites, 0.0);                                                              \
    std::vector<double> pol(n_sites, 0.0);                                                                 \
    std::vector<std::string> monomer_names(n_monomers, "h2o");                                             \
    std::vector<size_t> islocal = {1, 1, 1};                                                               \
    std::vector<double> charges{0.0000000000e+00, 5.7769308798e-01, 5.7615998121e-01, -1.1538530692e+00,   \
                                0.0000000000e+00, 5.7663564934e-01, 5.7823701694e-01, -1.1548726663e+00,   \
                                0.0000000000e+00, 5.7859034043e-01, 5.7709162608e-01, -1.1556819665e+00};  \
    for (int n = 0; n < n_monomers; ++n) {                                                                 \
        polfac[4 * n + 0] = 1.31000e+00;                                                                   \
        polfac[4 * n + 1] = 2.94000e-01;                                                                   \
        polfac[4 * n + 2] = 2.94000e-01;                                                                   \
        polfac[4 * n + 3] = 1.31000e+00;                                                                   \
        pol[4 * n + 0] = 1.31000e+00;                                                                      \
        pol[4 * n + 1] = 2.94000e-01;                                                                      \
        pol[4 * n + 2] = 2.94000e-01;                                                                      \
        pol[4 * n + 3] = 0.00000e+00;                                                                      \
    }                                                                                                      \
    std::vector<size_t> first_ind(n_monomers, 0);                                                          \
    for (int n = 1; n < n_monomers; ++n) {                                                                 \
        first_ind[n] = 4 * n;                                                                              \
    }                                                                                                      \
    std::vector<size_t> sites(n_monomers, 4);                                                              \
    std::vector<size_t> nats(n_monomers, 3);                                                               \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                       \
    std::vector<double> coords{1.2905344340e+00,  4.4014972470e+00,  3.8386732970e-02,  1.4006239730e+00,  \
                               5.0250416720e+00,  7.5623989970e-01,  3.3824898600e-01,  4.2277894090e+00,  \
                               -1.1389437460e-02, 1.1108490388e+00,  4.4974714307e+00,  1.8092325898e-01,  \
                               1.2780339070e+00,  1.6066904830e+00,  2.0362503520e-01,  1.6046328290e+00,  \
                               2.5191411460e+00,  1.8289029060e-01,  1.8551906800e+00,  1.1107870280e+00,  \
                               -3.7618181440e-01, 1.4708532944e+00,  1.6955622632e+00,  7.5497419614e-02,  \
                               -1.1351963630e+00, 3.0111682920e+00,  -3.0186702270e-02, -1.7552407620e+00, \
                               2.8389942210e+00,  -7.3812021870e-01, -5.2315192480e-01, 2.2604237960e+00,  \
                               -2.5263788390e-02, -1.1369031822e+00, 2.8142604400e+00,  -1.8017643338e-01};
#endif
