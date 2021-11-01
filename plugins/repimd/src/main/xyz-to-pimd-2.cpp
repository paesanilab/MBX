#include <cmath>
#include <cassert>
#include <cstdlib>

#include <map>
#include <fstream>
#include <iostream>
#include <exception>

#include "nhc.h"
#include "pimd_v2.h"
#include "io-xyz.h"
#include "io-utils.h"
#include "tools/mt19937.h"
#include "tools/constants.h"

//
// loads XYZ, generates a "frame" that can be used for PIMD
//

namespace {

struct mypimd : public kit::pimd {
    double force(double* x, double* f, MPI_Comm comm) { return 0.0; }
};

}  // namespace

int main(int argc, char** argv) {
    typedef std::map<std::string, double> mass_map_type;

    mass_map_type mass_map;

    mass_map["H"] = 1.007825032;
    mass_map["D"] = 2.01410178;
    mass_map["O"] = 15.994914619;
    mass_map["F"] = 18.998403162;
    mass_map["Cl"] = 34.968852682;
    mass_map["Br"] = 78.9183376;
    mass_map["I"] = 126.9044719;
    mass_map["Li"] = 7.0160034366;
    mass_map["Na"] = 22.989769282;
    mass_map["K"] = 38.9637064864;
    mass_map["Rb"] = 83.9114977282;
    mass_map["Cs"] = 132.905429;
    mass_map["Ne"] = 20.179;
    mass_map["Ar"] = 39.948;
    mass_map["C"] = 12.0107;
    mass_map["S"] = 32.065;
    mass_map["N"] = 14.00307400443;

    if (argc != 4) {
        std::cerr << "usage: xyz-to-pimd-2 T nbead file.xyz" << std::endl;
        return EXIT_FAILURE;
    }

    double T(0);
    size_t nbead(0);

    try {
        T = kit::io::to_double(argv[1]);
        nbead = kit::io::to_uint(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (T < 0.0) T = 0.0;

    if (nbead == 0) nbead = 1;

    // load XYZ

    std::vector<double> crd;
    std::vector<std::string> elements;

    try {
        assert(argv[3]);
        std::ifstream ifs(argv[3]);
        std::string comment;
        kit::io::load_xyz(ifs, comment, elements, crd);
        // PB debugging
        // for (auto i = crd.begin(); i != crd.end(); ++i)
        //    std::cout << *i << ' ';
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    size_t natom = elements.size();
    // std::cout << "elements.size() " << elements.size() << std::endl;
    // std::cout << "crd.size() " << crd.size() << std::endl
    ;
    double* xyz = new double[3 * natom];
    std::copy(crd.begin(), crd.end(), xyz);

    double* mass = new double[natom];
    for (size_t n = 0; n < natom; ++n) {
        mass_map_type::const_iterator mit = mass_map.find(elements[n]);
        if (mit == mass_map.end()) {
            std::cerr << " ** Error ** : could not figure out atomic mass of '" << elements[n] << "' element"
                      << std::endl;
            return EXIT_FAILURE;
        }
        mass[n] = mit->second;
    }

    int seed;
    {
        std::ifstream ifs("/dev/random", std::ios::in | std::ios::binary);
        ifs.read((char*)&seed, sizeof(seed));
        std::cerr << "PRG seed = " << seed << std::endl;
    }

    mypimd sim;
    kit::mt19937 prg(seed);

    sim.init1(natom, nbead, T, mass, xyz, prg);
    sim.save(std::cout, 0.0);

    delete[] xyz;
    return EXIT_SUCCESS;
}
