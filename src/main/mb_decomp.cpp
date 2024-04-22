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

namespace {

static std::vector<bblock::System> systems;
static int max_order = -1; // maximum subset order
static bool verbose = false;
static bool do_decomp = true;

void show_usage();
void parse_command_line(int, char**);
void getSubset(std::vector<std::vector<std::vector<size_t>>>& subset, std::vector<size_t> monomer_index, int max_order = -1);

}  // namespace

int main(int argc, char** argv) {

    parse_command_line(argc, argv);

    // Check number of arguments
    if (argc - optind < 1) {
        show_usage();
        return 0;
    }

    // Load the nrg file
    try {

        tools::ReadNrg(argv[optind], systems);

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    for (size_t ii = 0; ii < systems.size(); ii++) {

        if(verbose) std::cout << "--- frame " << ii << " ---" << std::endl;

        // Load JSON file
        if (argc - optind > 1) {
            systems[ii].SetUpFromJson(argv[optind+1]);
        } else {
            systems[ii].SetUpFromJson();
        }
    
        // coordinates
        std::vector<double> coordinates = systems[ii].GetRealXyz();
        // a vector with the atom names
        std::vector<std::string> atom_names = systems[ii].GetRealAtomNames();
        // a vector with the monomer ids
        std::vector<std::string> monomer_ids = systems[ii].GetMonId();
        // and the number of atoms in each monomer
        std::vector<size_t> monomer_number_of_atoms = systems[ii].GetMonNumAt();

        std::vector<size_t> monomer_index;
        std::vector<size_t> first_index;
        size_t count = 0;
        for (size_t mm = 0; mm < monomer_ids.size() ; mm++) {
            monomer_index.push_back(mm);
            first_index.push_back(count);
            count += monomer_number_of_atoms[mm];
        }

        // calculate subset
        std::vector<std::vector< std::vector<size_t> >> subset;
        getSubset(subset, monomer_index, max_order);

        // dictionary: subset indicies {i,j,k} -> energy
        std::map< std::vector<size_t>,double > energies;

        for (size_t nn = 0; nn < subset.size(); nn++) {
            // loop through all (nn+1)-th order subset
            for (size_t jj = 0; jj < subset[nn].size() ; jj++) {

                // set up subsystem
                bblock::System subsystem;

                // loop through subsystem monomers
                for (size_t kk = 0; kk < subset[nn][jj].size() ; kk++) {
                    size_t mm = subset[nn][jj][kk];
                    std::vector<double> xyz(3 * monomer_number_of_atoms[mm]);
                    std::vector<std::string> vAtNames(monomer_number_of_atoms[mm]);

                    std::copy(coordinates.begin() + 3 * first_index[mm], coordinates.begin() + 3 * (first_index[mm] + monomer_number_of_atoms[mm]), xyz.begin());
                    std::copy(atom_names.begin() + count, atom_names.begin() + first_index[mm] + monomer_number_of_atoms[mm], vAtNames.begin());
                    std::string id = monomer_ids[mm];
                    subsystem.AddMonomer(xyz, vAtNames, id);

                    if(verbose) std::cout << mm << ",";
                }

//                subsystem.Initialize(); // in system.cpp, it seems this line does not have any effect before json is read.

                // Load JSON file
                if (argc - optind > 1) {
                    subsystem.SetUpFromJson(argv[optind+1]);
                } else {
                    subsystem.SetUpFromJson();
                }

                // calculate subsystem energy
                energies[subset[nn][jj]] = subsystem.Energy(false);

                if(do_decomp) {

                    // at this point, energies[subset[nn][jj]] stores total energy
                    // energies[subset[<nn][:]] stores many-body energy

                    // calculate many-body decomp
                    std::vector<std::vector< std::vector<size_t> >> subSubset;
                    getSubset(subSubset, subset[nn][jj]);
                    for (size_t nnn = 0; nnn < subSubset.size()-1; nnn++) {
                        for (size_t jjj = 0; jjj < subSubset[nnn].size() ; jjj++) {
                            energies[subset[nn][jj]] -= energies[subSubset[nnn][jjj]];
                        }
                    }

                    // at this point, energies[subset[nn][jj]] stores many-body energy

                }

                std::cout << " " << std::setprecision(10) << energies[subset[nn][jj]];
                std::cout.flush();
                if(verbose) std::cout << std::endl;
            }
        }
        if(!verbose) std::cout << std::endl;
    }

    return 0;
}

namespace {

////////////////////////////////////////////////////////////////////////////////

// input : subset [target output]
//   subset[0] : 1-mers
//   subset[1] : 2-mers
//   ...
//   e.g. subset[2][0] = { monomer_index[i] , monomer_index[j] , monomer_index[k] } ; // i<j<k
//
// input : monomer_index (e.g. {0,1,2,3,...})
// input : max_order
void getSubset(std::vector<std::vector< std::vector<size_t> >>& subset, std::vector<size_t> monomer_index, int max_order) {

    subset.clear();

    // first, collect subset using vector index of monomer_index as label
    size_t nn_max =  ( max_order < 0 || (size_t) max_order > monomer_index.size() ) ? monomer_index.size() : (size_t) max_order;
    for (size_t nn = 0; nn < nn_max; nn++) {

        // collect (nn+1)-th order subset in subset[nn]
        std::vector< std::vector<size_t> > nmers;

        if (nn == 0) {
            // first add monomers to subset[0]
            for (size_t mm = 0; mm < monomer_index.size() ; mm++) { // mm : monomers
                std::vector<size_t> id;
                id.push_back(mm);
                nmers.push_back(id);
            }
        } else {
            // append monomer to subset[nn-1] to get subset[nn]
            for (size_t jj = 0; jj < subset[nn-1].size() ; jj++) { // jj : subset[nn-1]
                for (size_t mm = subset[nn-1][jj].back()+1; mm < monomer_index.size() ; mm++) {
                    std::vector<size_t> id = subset[nn-1][jj];
                    id.push_back(mm);
                    nmers.push_back(id);
                }
            }
        }

        subset.push_back(nmers);

    }

    // then, convert vector index of monomer_index to the "index" value stored in monomer_index
    for (size_t nn = 0; nn < subset.size(); nn++) {
        for (size_t jj = 0; jj < subset[nn].size() ; jj++) {
            for (size_t kk = 0; kk < subset[nn][jj].size() ; kk++) {
                subset[nn][jj][kk] = monomer_index[subset[nn][jj][kk]];
            }
        }
    }
}

//----------------------------------------------------------------------------//

void show_usage()
{
    std::cerr <<
    "\n"
    "Usage: ./mb_decomp [options]  <input.nrg> [mbx.json]\n"
    "Computes energies for subsystems for the given system(s).\n"
    "\n"
    "Options:\n"
    "    -h      , help\n"
    "    -v      , verbose\n"
    "    -e      , skip_decomp (won't perform many-body decomposition)\n"
    "    -o <int>, max_order (default: -1, calculate for all subsystems)\n"
    "\nIf -v is set, program prints monomer indices of each subsystem, followed"
    "\nby the energy value. Every subsystem uses one line. Otherwise, energies"
    "\nof subsystems are printed without line-breaks. Each frame uses one line."
    "\nIf -e is set, program prints singlepoint total energies of subsystems."
    "\nOtherwise, program prints many-body energies of subsystems.\n"
    "\n-o <int> if set, calculate for subsystems upto this order."
    "\n"
    "\nExample:"
    "\n$ ./mb_decomp -v -o 2 -e input.nrg"
    "\n\n";
}

//----------------------------------------------------------------------------//

void parse_command_line(int argc, char** argv)
{
    int c;

    static const char short_options[] = "hveo:";

    while (true) {

        c = getopt(argc, argv, short_options);

        if (c == -1)
            break;

        switch (c) {
            case 'h':
                show_usage();
                std::exit(EXIT_SUCCESS);
            case 'v':
                verbose = true;
                break;
            case 'e':
                do_decomp = false;
                break;
            case 'o':
                max_order = atoi(optarg);
                break;
            case '?':
                std::exit(EXIT_FAILURE);
                break;
            default:
                assert(false); // should not be reached
        }
    } // while (true)
}

} // namespace
