#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

//#define NUMGRADS
//#define PRINT_GRADS
//#define PRINT_VIRIAL
namespace {

static std::vector<bblock::System> systems;

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.nrg> [mbx.json]" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
        ifs.close();

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> box;

    if (argc > 2) {
        systems[0].SetUpFromJson(argv[2]);
    } else {
        systems[0].SetUpFromJson();
    }

    std::vector<size_t> dimers = systems[0].GetPairList(2, 9.0, 0, 1000, false);
    std::vector<size_t> trimers = systems[0].GetPairList(3, 4.5, 0, 1000, false);

    std::cout << "Full system: " << dimers.size() / 2 << " dimers and " << trimers.size() / 3 << " trimers."
              << std::endl;

    // Create fake ghost systems
    std::vector<std::string> monid = systems[0].GetMonId();
    std::vector<size_t> ats = systems[0].GetAtomsVector();
    std::vector<std::string> atoms = systems[0].GetAtomNames();
    std::vector<double> xyz = systems[0].GetRealXyz();

    size_t half = monid.size() / 2;
    std::cout << "splitting at " << half << std::endl;

    bblock::System s1, s2;

    // System 1: 1st half real, second ghost
    // System 2: 2nd half real, first ghost
    size_t fi = 0;
    for (size_t i = 0; i < half; i++) {
        std::vector<double> c(xyz.begin() + 3 * fi, xyz.begin() + 3 * fi + 3 * ats[i]);
        std::vector<std::string> a(atoms.begin() + fi, atoms.begin() + fi + ats[i]);
        s1.AddMonomer(c, a, monid[i], 1, fi + 1);
        s2.AddMonomer(c, a, monid[i], 0, fi + 1);
        fi += ats[i];
    }

    for (size_t i = half; i < monid.size(); i++) {
        std::vector<double> c(xyz.begin() + 3 * fi, xyz.begin() + 3 * fi + 3 * ats[i]);
        std::vector<std::string> a(atoms.begin() + fi, atoms.begin() + fi + ats[i]);
        s1.AddMonomer(c, a, monid[i], 0, fi + 1);
        s2.AddMonomer(c, a, monid[i], 1, fi + 1);
        fi += ats[i];
    }

    s1.Initialize();
    s2.Initialize();
    s1.SetUpFromJson(argv[2]);
    s2.SetUpFromJson(argv[2]);

    dimers = s1.GetPairList(2, 9.0, 0, 1000, true);
    std::vector<size_t> dimers_local = s1.GetPairList(2, 9.0, 0, 1000, false);
    trimers = s1.GetPairList(3, 4.5, 0, 1000, true);
    std::vector<size_t> trimers_local = s1.GetPairList(3, 4.5, 0, 1000, false);

    std::cout << "s1: " << dimers.size() / 2 << " dimers and " << trimers.size() / 3 << " trimers." << std::endl;
    std::cout << "s1: " << dimers_local.size() / 2 << " local dimers and " << trimers_local.size() / 3
              << " local trimers." << std::endl;

    dimers = s2.GetPairList(2, 9.0, 0, 1000, true);
    dimers_local = s2.GetPairList(2, 9.0, 0, 1000, false);
    trimers = s2.GetPairList(3, 4.5, 0, 1000, true);
    trimers_local = s2.GetPairList(3, 4.5, 0, 1000, false);

    std::cout << "s2: " << dimers.size() / 2 << " dimers and " << trimers.size() / 3 << " trimers." << std::endl;
    std::cout << "s2: " << dimers_local.size() / 2 << " local dimers and " << trimers_local.size() / 3
              << " local trimers." << std::endl;

    double e2b_1 = s1.TwoBodyEnergy(true, true);
    double e2b_1l = s1.TwoBodyEnergy(true);
    double e2b_2 = s2.TwoBodyEnergy(true, true);
    double e2b_2l = s2.TwoBodyEnergy(true);

    double e2b = systems[0].TwoBodyEnergy(true, false);

    std::cout << "2B: full " << e2b << " , s1 " << e2b_1 << " , s2 " << e2b_2 << std::endl;
    std::cout << "2B: s1l " << e2b_1l << " , s2l " << e2b_2l << std::endl;

    return 0;
}
