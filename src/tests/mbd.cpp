#include "bblock/system.h"
#include "io_tools/read_xyz.h"
#include "io_tools/read_nrg.h"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    std::vector<bblock::System> my_sys_t;
    bblock::System my_sys_d1;
    bblock::System my_sys_d2;
    bblock::System my_sys_d3;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << "<input.nrg> [<mbx.json> <configs.xyz>]" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;

    bool use_xyz = false;

    try {
        // Checking each one of the input files in the command line
        tools::ReadNrg(argv[1], my_sys_t);

        if (argc > 3) {
            std::cout << "Reading xyz file...\n";
            tools::ReadXYZ(argv[3], ats, coords, boxes);
            use_xyz = true;
        }

        if (argc > 2) {
            my_sys_t[0].SetUpFromJson(argv[2]);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::vector<double> threebody(ats.size(), 0.0);

    // We need to set up the different systems for dimers and monomers
    // Get info about monomers we have.
    std::vector<size_t> num_at = my_sys_t[0].GetMonNumAt();
    std::vector<std::string> mon_ids = my_sys_t[0].GetMonId();
    std::vector<double> xyz_nrg_coords = my_sys_t[0].GetRealXyz();
    std::vector<std::string> atom_names = my_sys_t[0].GetRealAtomNames();

    // Set up the other systems
    std::vector<double> xyz;
    std::vector<std::string> m_name;
    std::vector<std::string> atoms;
    std::string monid;

    // Mon 1
    xyz = std::vector<double>(3 * (num_at[0]));
    atoms = std::vector<std::string>(num_at[0]);

    std::copy(xyz_nrg_coords.begin(), xyz_nrg_coords.begin() + 3 * (num_at[0]), xyz.begin());
    std::copy(atom_names.begin(), atom_names.begin() + num_at[0], atoms.begin());

    my_sys_d1.AddMonomer(xyz, atoms, mon_ids[0]);
    my_sys_d2.AddMonomer(xyz, atoms, mon_ids[0]);

    // Mon 2
    xyz = std::vector<double>(3 * (num_at[1]));
    atoms = std::vector<std::string>(num_at[1]);

    std::copy(xyz_nrg_coords.begin() + 3 * num_at[0], xyz_nrg_coords.begin() + 3 * (num_at[0] + num_at[1]),
              xyz.begin());
    std::copy(atom_names.begin() + num_at[0], atom_names.begin() + num_at[0] + num_at[1], atoms.begin());

    my_sys_d1.AddMonomer(xyz, atoms, mon_ids[1]);
    my_sys_d3.AddMonomer(xyz, atoms, mon_ids[1]);

    // Mon 3
    xyz = std::vector<double>(3 * (num_at[2]));
    atoms = std::vector<std::string>(num_at[2]);

    std::copy(xyz_nrg_coords.begin() + 3 * (num_at[0] + num_at[1]), xyz_nrg_coords.end(), xyz.begin());
    std::copy(atom_names.begin() + num_at[0] + num_at[1], atom_names.end(), atoms.begin());

    my_sys_d2.AddMonomer(xyz, atoms, mon_ids[2]);
    my_sys_d3.AddMonomer(xyz, atoms, mon_ids[2]);

    // initialize systems
    my_sys_d1.Initialize();
    my_sys_d2.Initialize();
    my_sys_d3.Initialize();

    if (argc > 2) {
        my_sys_d1.SetUpFromJson(argv[2]);
        my_sys_d2.SetUpFromJson(argv[2]);
        my_sys_d3.SetUpFromJson(argv[2]);
    }

    std::cout << std::scientific;
    if (!use_xyz) {
        double en3b = my_sys_t[0].Energy(false) - my_sys_d1.Energy(false) - my_sys_d2.Energy(false) -
                      my_sys_d3.Energy(false) + my_sys_t[0].OneBodyEnergy(false);
        std::cout << "E3B = " << std::setprecision(8) << en3b << std::endl;
        return 0;
    }

    std::cout << "Calculating the three-body energy for " << ats.size() << " frames\n";
    // If an xyz was provided, use it.
    for (size_t i = 0; i < ats.size(); i++) {
        xyz = std::vector<double>(3 * (num_at[0] + num_at[1]));
        std::copy(coords[i].begin(), coords[i].begin() + 3 * (num_at[0] + num_at[1]), xyz.begin());
        my_sys_d1.SetRealXyz(xyz);

        xyz = std::vector<double>(3 * (num_at[2] + num_at[1]));
        std::copy(coords[i].begin() + 3 * num_at[0], coords[i].end(), xyz.begin());
        my_sys_d3.SetRealXyz(xyz);

        xyz = std::vector<double>(3 * (num_at[0] + num_at[2]));
        std::copy(coords[i].begin(), coords[i].begin() + 3 * num_at[0], xyz.begin());
        std::copy(coords[i].begin() + 3 * (num_at[0] + num_at[1]), coords[i].end(), xyz.begin() + 3 * num_at[0]);
        my_sys_d2.SetRealXyz(xyz);

        my_sys_t[0].SetRealXyz(coords[i]);

        threebody[i] = my_sys_t[0].Energy(false) - my_sys_d1.Energy(false) - my_sys_d2.Energy(false) -
                       my_sys_d3.Energy(false) + my_sys_t[0].OneBodyEnergy(false);
        ;
        if (i % 1000 == 0) std::cout << "Frame " << i << std::endl;
    }

    std::ofstream off("threebodyenergies.dat");

    for (size_t i = 0; i < ats.size(); i++) {
        off << std::scientific << std::setprecision(8) << threebody[i] << std::endl;
    }

    off.close();

    return 0;
}
