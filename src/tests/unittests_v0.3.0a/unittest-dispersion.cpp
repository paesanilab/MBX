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

#include "tools/testutils.h"

#include "potential/dispersion/dispersion.h"
#include "tools/math_tools.h"
#include "setup_monomer_mix.h"
#include "json/json.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("dispersion::Initialize") {
    std::vector<double> c6lr = {
        1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00,
        1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01,
        1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01,
        1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01};
    std::vector<double> sys_xyz = {
        6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01,
        -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  2.4803292099e+00,
        7.5103875900e-01,  -2.9043390394e+00, 2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00,
        2.7020706245e+00,  -3.5351972400e-01, -3.2106052081e+00, -1.4243133388e+00, 7.7214187430e-01,
        -2.3329941557e+00, -2.4475497469e+00, 1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01,
        2.6460559090e-01,  -2.5998516399e+00, 6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01,
        7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,
        -1.0197806380e+00, 5.1568113210e-01,  3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01,
        3.2992897240e+00,  -3.4756612500e+00, 1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00,
        -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,
        -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00,
        -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,
        -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00};
    std::vector<double> box = {2.0000000000e+01, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01,
                               0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01};
    std::vector<size_t> num_atoms = {3, 3, 3, 3, 3, 3, 3};
    std::vector<size_t> islocal = {1, 1, 1, 1, 1, 1, 1};
    std::vector<std::pair<std::string, size_t> > mon_type_count = {{"h2o", 2}, {"co2", 5}};
    std::vector<std::string> mon_id = {"h2o", "h2o", "co2", "co2", "co2", "co2", "co2"};
    bool do_grads = true;

    std::vector<double> internal_c6lr_expected = {
        1.5405233572e+01, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 4.4825869765e+00, 4.4825869765e+00,
        1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.3042057313e+01,
        1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01,
        1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01};
    std::vector<double> internal_xyz_expected = {
        6.6630444410e-01,  2.4803292099e+00,  -3.8357176030e-01, 7.5103875900e-01,  1.1519802350e-01,
        -2.9043390394e+00, 1.7838183644e+00,  2.2674715176e+00,  -1.9222069500e-01, 1.8651909097e+00,
        -1.7587628680e-01, -2.6082983571e+00, -4.4811475090e-01, 2.7020706245e+00,  -5.7997649630e-01,
        -3.5351972400e-01, 4.1069507510e-01,  -3.2106052081e+00, -1.4243133388e+00, 6.4986549630e-01,
        5.1568113210e-01,  -9.1278996460e-01, -1.4297465600e+00, 7.7214187430e-01,  3.4030903031e+00,
        3.8303951459e+00,  1.3711133940e+00,  -2.3744893640e-01, -2.3329941557e+00, -6.8029207450e-01,
        -4.0222969886e+00, -2.9257875690e+00, 1.1236431040e+00,  -2.4475497469e+00, 7.9296435600e-02,
        -3.7522694700e-01, -4.0252047300e-01, -7.5808708930e-01, 1.2692755042e+00,  2.4412841935e+00,
        3.2992897240e+00,  1.9907698680e+00,  3.9350777550e-01,  -2.0709757555e+00, -3.3312155960e-01,
        -3.4756612500e+00, -3.4499780000e+00, 1.4232346440e+00,  -4.0159255460e-01, 1.2145816282e+00,
        1.3973748074e+00,  -1.7848310380e+00, -2.2258007790e+00, 2.6460559090e-01,  4.3686301334e+00,
        4.3616623515e+00,  1.3696631160e+00,  -6.7917366630e-02, -2.5998516399e+00, -1.0197806380e+00,
        -4.5737147405e+00, -3.3238997190e+00, 1.6302588920e+00};
    std::vector<size_t> internal_is_local_atom_expected = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    std::vector<double> box_ABCabc = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};

    // Declare and initialize dispersion
    disp::Dispersion d;
    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);

    // Get everything and ensure that is correct
    std::vector<size_t> getislocal = d.GetIsLocal();
    std::vector<std::string> getmonids = d.GetMonIds();
    std::vector<size_t> getnumatsvec = d.GetNumAtomsVector();
    std::vector<std::pair<std::string, size_t> > getmontypecount = d.GetMonTypeCount();
    bool getdograds = d.GetDoGrads();
    std::vector<double> getbox = d.GetBox();
    std::vector<double> getboxabc = d.GetBoxAbc();
    std::vector<double> getboxinverse = d.GetBoxInverse();
    std::vector<double> getsysxyz = d.GetSystemXyz();
    std::vector<double> getinternalxyz = d.GetInternalXyz();
    std::vector<double> getc6lrsys = d.GetSystemC6LongRange();
    std::vector<double> getinternalc6lr = d.GetInternalC6LongRange();
    std::vector<size_t> getislocalatom = d.GetIsLocalAtom();

    // Require that everything matches what is expected
    REQUIRE(getdograds == do_grads);
    REQUIRE(VectorsAreEqual(getislocal, islocal));
    REQUIRE(VectorsAreEqual(getmonids, mon_id));
    REQUIRE(VectorsAreEqual(getnumatsvec, num_atoms));
    REQUIRE(VectorsAreEqual(getmontypecount, mon_type_count));
    REQUIRE(VectorsAreEqual(getbox, box, TOL));
    REQUIRE(VectorsAreEqual(getboxabc, box_ABCabc, TOL));
    REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
    REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
    REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
    REQUIRE(VectorsAreEqual(getc6lrsys, c6lr, TOL));
    REQUIRE(VectorsAreEqual(getinternalc6lr, internal_c6lr_expected, TOL));
    REQUIRE(VectorsAreEqual(getislocalatom, internal_is_local_atom_expected));
}
