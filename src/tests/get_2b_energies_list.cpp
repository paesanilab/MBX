#include "bblock/system.h"
#include "io_tools/read_xyz.h"
#include "io_tools/read_nrg.h"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    std::vector<bblock::System> sys_m1;
    std::vector<bblock::System> sys_m2;
    std::vector<bblock::System> sys_dim;

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << "<dimer.nrg> <configs.xyz> <mbx.json>" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;

    try {
        // Checking each one of the input files in the command line
        tools::ReadNrg(argv[1], sys_dim);
        tools::ReadXYZ(argv[2], ats, coords, boxes);

        if (argc == 4) {
            sys_dim[0].SetUpFromJson(argv[3]);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::vector<double> twobody(ats.size(), 0.0);

    // Get the 2b energy for each config in xyz
    for (size_t i = 0; i < ats.size(); i++) {
        sys_dim[0].SetRealXyz(coords[i]);
        double e1b = sys_dim[0].OneBodyEnergy(false);
        double edim = sys_dim[0].Energy(false);
        twobody[i] = edim - e1b;
        if (i % 1000 == 0) std::cout << i << std::endl;
    }

    std::ofstream off("twobodyenergies.dat");

    for (size_t i = 0; i < ats.size(); i++) {
        off << twobody[i] << std::endl;
    }

    off.close();

    return 0;
}
