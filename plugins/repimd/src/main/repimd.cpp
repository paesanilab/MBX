#include <mpi.h>

#include <cmath>
#include <cassert>
#include <cstdlib>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

// For MBX
#include "io_tools/read_nrg.h"
#include "bblock/system.h"

// For frame IO
#include "io-xyz.h"
#include "io-frame.h"
#include "io-utils.h"

// For REPIMD
#include "sphere.h"

#include "nhc.h"
#include "pimd_v2.h"
#include "tools/mt19937.h"

#include "tools/constants.h"

//=======================//
// Replica-Exchange PIMD //
//=======================//

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//
// cmdline arguments |
//-------------------+

static size_t nstep = 0;
static size_t initial_step = 0;
static size_t nprint = 100;
static size_t ntraj = 0;
static size_t nreposition = 100;
static size_t nvirials = 0;

static size_t nexchange = 100;
static size_t nreplicas = 1;

static size_t nrestart = 1;
static size_t rst = 0;

static double dt = 0.0001;  // ps

static double trap_R = 5.0;    // A
static double trap_U = 100.0;  // kcal/mol

static std::vector<std::string> atnames;

static std::vector<bblock::System> systems;

static std::string initial = "repimd.initial";
static std::string final = "repimd.final";
static std::string restart = "repimd.restart";
static std::string trajectory = "repimd.trajectory";
static std::string output = "repimd.output";
static std::string virials = "repimd.virials";
static std::string nrg = "input.nrg";
static std::string json = "mbx.json";

void show_usage();
void parse_cmdline(int, char**);

//----------------------------------------------------------------------------//
// PIMD class |
//------------+

struct my_pimd : public kit::pimd {
    my_pimd();
    ~my_pimd();

    double force(double*, double*, MPI_Comm);

    void step(const double&, MPI_Comm);
    void set_COM_at_origin();
    void try_exchange(std::ostream&, size_t);
    void print_energies(std::ostream&, size_t);
    void save_positions(std::ostream&, size_t);
    void save_virials(std::ostream&, size_t);
    std::string mass2name(double mass);

    double m_Econ_sum;
    double* m_virials;  // per-particle centroid virials
    void compute_virials();

    kit::sphere m_trap;
};

//----------------------------------------------------------------------------//
// communications |
//----------------+

static MPI_Comm comm_replica = MPI_COMM_NULL;
static MPI_Comm comm_masters = MPI_COMM_NULL;

static int comm_replica_rank = MPI_UNDEFINED;
static int comm_replica_size = MPI_UNDEFINED;

static int comm_masters_rank = MPI_UNDEFINED;
static int comm_masters_size = MPI_UNDEFINED;

static int replica_id = 0;

void setup_comms(MPI_Comm);

//----------------------------------------------------------------------------//
// exchanges |
//-----------+

static kit::mt19937::data_type re_prg_seed = 278400;
static kit::mt19937 re_prg;  // must use same seed for all replicas
void print_exchange(std::ostream&, size_t, const double& Told, const double& Tpartner, const double& Tnew);

//----------------------------------------------------------------------------//

}  // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    const double wtime_start = MPI_Wtime();

    my_pimd pimd;
    try {
        // Get Command line arguments
        parse_cmdline(argc, argv);

        // Setup MPI
        setup_comms(MPI_COMM_WORLD);

        // Initialize the beads
        pimd.load(initial.c_str(), comm_replica);

        // Initialize the system
        char nrg_char[nrg.size() + 1];
        strcpy(nrg_char, nrg.c_str());
        tools::ReadNrg(nrg_char, systems);

        // Read Json adn set up:
        char json_char[json.size() + 1];
        strcpy(json_char, json.c_str());
        systems[0].SetUpFromJson(json_char);

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;

        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

#ifdef DEBUG
    std::cerr << "Initial time: " << pimd.time0() << std::endl;
    std::cerr << "Time step = " << dt << std::endl;
#endif

    nrestart = ntraj;

    initial_step = (size_t)(pimd.time0() / dt);

    assert(pimd.natom() > 0);
    for (int i = 0; i < pimd.natom(); i++) {
        atnames.push_back(pimd.mass2name(pimd.mass(i)));
    }

    // check the input

    dt = std::max(dt, 0.0);

    trap_R = std::max(1.0, trap_R);
    trap_U = std::max(0.0, trap_U);

    // initialize & report

    re_prg.seed(re_prg_seed);

    pimd.m_trap.setup(trap_R, trap_U);
    pimd.step(0.0, comm_replica);  // fixes the energies

    if (comm_replica_rank == 0) pimd.m_virials = new double[pimd.natom()];

    std::ofstream os;
    std::ofstream os_traj;
    std::ofstream os_virials;

    if (comm_replica_rank == 0) {
        os.open(output.c_str());

        if (!os) {
            std::cerr << " ** Error ** : could not open '" << output << "' for writing " << std::endl;

            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        if (ntraj > 0) os_traj.open(trajectory.c_str());

        if (!os_traj) {
            std::cerr << " ** Error ** : could not open '" << trajectory << "' for writing " << std::endl;

            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        if (nvirials > 0) os_virials.open(virials.c_str());

        if (!os_virials) {
            std::cerr << " ** Error ** : could not open '" << virials << "' for writing " << std::endl;

            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        os << "# replica exchange PIMD ["
           //<< PACKAGE_VERSION << "]"
           << "\n# === this is replica " << (replica_id + 1) << " of " << nreplicas << " running on "
           << comm_replica_size << " nodes ==="
           << "\n#"
           << "\n#     PRG seed = " << re_prg_seed << "\n#        nstep = " << nstep << "\n#       nprint = " << nprint
           << "\n#        ntraj = " << ntraj << "\n#     nvirials = " << nvirials
           << "\n#  nreposition = " << nreposition << "\n#    nexchange = " << nexchange
           << "\n#           t0 = " << pimd.time0() << " ps"
           << "\n#           dt = " << dt << " ps"
           << "\n#            T = " << pimd.temperature() << " K"
           << "\n#       trap R = " << pimd.m_trap.R() << " A"
           << "\n#       trap U = " << pimd.m_trap.U() << " kcal/mol"
           << "\n#"
           << "\n#        natom = " << pimd.natom() << "\n#        nbead = " << pimd.nbead() << "\n#"
           << "\n#      initial = '" << initial << "'"
           << "\n#        final = '" << final << "'"
           << "\n#       output = '" << output << "'"
           << "\n#   trajectory = '" << trajectory << "'"
           << "\n#      virials = '" << virials << "'"
           << "\n#          nrg = '" << nrg << "'"
           << "\n#         json = '" << json << "'";

        os << std::setprecision(7) << std::scientific
           << "\n#\n# step time Epot-mean Espring Ekin-centroid"
              " Ekin-fict Etrap NHC-invariant\n#\n";

        print_exchange(os, initial_step, pimd.temperature(), -1.0, pimd.temperature());
        pimd.compute_virials();  // virials are needed in ::print_energies()
        pimd.print_energies(os, initial_step);

    }  // comm_replica_rank == 0;

    // MD loop

    for (size_t step = initial_step; step < nstep; ++step) {
        pimd.step(dt, comm_replica);

        // TODO Add restart here?
        if (comm_replica_rank == 0) {
            const bool need_to_print_energies = (nprint > 0 && step > 0 && (step + 1) % nprint == 0);

            const bool need_to_save_virials = (nvirials > 0 && step > 0 && (step + 1) % nvirials == 0);

            if (need_to_print_energies || need_to_save_virials) pimd.compute_virials();

            if (nreposition > 0 && step % nreposition == 0) pimd.set_COM_at_origin();

            if (need_to_print_energies) pimd.print_energies(os, step + 1);

            if (ntraj > 0 && step > 0 && (step + 1) % ntraj == 0) pimd.save_positions(os_traj, step + 1);

            if (need_to_save_virials) pimd.save_virials(os_virials, step + 1);

            if (step > 0 && step % nexchange == 0 && comm_masters_rank != MPI_UNDEFINED) pimd.try_exchange(os, step);

            // Saving restart
            if ((step + 1) % nrestart == 0) {
                std::ofstream ofs(restart.c_str());

                if (!ofs) {
                    std::cerr << " ** Error ** : could not open '" << restart << "' for writing" << std::endl;

                    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                }

                pimd.save(ofs, dt * (step + 1));
            }
        }  // comm_replica_rank == 0

    }  // MD loop

    // save final

    if (comm_replica_rank == 0) {
        std::ofstream ofs(final.c_str());

        if (!ofs) {
            std::cerr << " ** Error ** : could not open '" << final << "' for writing" << std::endl;

            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        pimd.save(ofs, dt * nstep);

        os.precision(9);
        const double elapsed_seconds = MPI_Wtime() - wtime_start;
        os << "#\n# done (" << std::setw(15) << elapsed_seconds << " sec)" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_GETOPT_H
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif  // _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>
#else
#include <unistd.h>
#endif  // HAVE_GETOPT_H

extern "C" {
extern char* optarg;
extern int optind, opterr, optopt;
}  // extern "C"

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

void show_usage() {
    std::cout << "\n"
                 "Usage: repimd [options]\n"
                 "\n"
                 "Options:\n"
                 "  -h --help\n"
                 "  -n --nstep\n"
                 "  -p --nprint\n"
                 "  -t --ntraj\n"
                 "  -s --timestep\n"
                 "  -R --trap-radius\n"
                 "  -U --trap-strength\n"
                 "  -r --restart\n"
                 "  -i --initial\n"
                 "  -f --final\n"
                 "  -o --output\n"
                 "  -j --trajectory\n"
                 "  -C --nreposition\n"
                 "  -N --nexchange\n"
                 "  -T --nreplicas\n"
                 "  -S --seed\n"
                 "  -v --nvirials\n"
                 "  -F --virials\n"
                 "  -I --nrg\n"
                 "  -J --json\n"
                 "\n";
}

//----------------------------------------------------------------------------//

void parse_cmdline(int argc, char** argv) {
    using kit::io::to_double;
    using kit::io::to_int;
    using kit::io::to_uint;

    int c;

    static const char short_options[] = "hn:p:t:s:R:U:r:i:f:o:j:C:N:T:S:v:F:I:J:";

#ifdef HAVE_GETOPT_H
    static const struct option long_options[] = {
        {"help", 0, 0, 'h'},          {"nstep", 1, 0, 'n'},     {"nprint", 1, 0, 'p'},
        {"ntraj", 1, 0, 't'},         {"timestep", 1, 0, 's'},  {"trap-radius", 1, 0, 'R'},
        {"trap-strength", 1, 0, 'U'}, {"restart", 1, 0, 'r'},   {"initial", 1, 0, 'i'},
        {"final", 1, 0, 'f'},         {"output", 1, 0, 'o'},    {"trajectory", 1, 0, 'j'},
        {"nreposition", 1, 0, 'C'},   {"nexchange", 1, 0, 'N'}, {"nreplicas", 1, 0, 'T'},
        {"seed", 1, 0, 'S'},          {"nvirials", 1, 0, 'v'},  {"virials", 1, 0, 'F'},
        {"nrg", 1, 0, 'I'},           {"json", 1, 0, 'J'},      {0, 0, 0, 0}};
#endif  // HAVE_GETOPT_H

    while (true) {
#ifdef HAVE_GETOPT_H
        c = getopt_long(argc, argv, short_options, long_options, 0);
#else
        c = getopt(argc, argv, short_options);
#endif  // HAVE_GETOPT_H

        if (c == -1) break;

        switch (c) {
            case 'h':
                show_usage();
                MPI_Finalize();
                std::exit(EXIT_SUCCESS);
            case 'n':
                nstep = to_uint(optarg);
                break;
            case 'p':
                nprint = to_uint(optarg);
                break;
            case 't':
                ntraj = to_uint(optarg);
                break;
            case 'v':
                nvirials = to_uint(optarg);
                break;
            case 's':
                dt = to_double(optarg);
                break;
            case 'R':
                trap_R = to_double(optarg);
                break;
            case 'U':
                trap_U = to_double(optarg);
                break;
            case 'r':
                rst = to_uint(optarg);
                break;
            case 'i':
                initial = optarg;
                break;
            case 'f':
                final = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'j':
                trajectory = optarg;
                break;
            case 'F':
                virials = optarg;
                break;
            case 'C':
                nreposition = to_uint(optarg);
                break;
            case 'N':
                nexchange = to_uint(optarg);
                break;
            case 'T':
                nreplicas = to_uint(optarg);
                break;
            case 'S':
                re_prg_seed = to_uint(optarg);
                break;
            case 'I':
                nrg = optarg;
                break;
            case 'J':
                json = optarg;
                break;
            case '?':
                std::exit(EXIT_FAILURE);
                break;
            default:
                assert(false);  // should not be reached
        }
    }  // while (true)

    if (optind < argc) {
        std::cerr << " ** Warning ** : unexpected command-line arguments:";
        for (; optind < argc; ++optind) std::cerr << " '" << argv[optind] << "'";
        std::cerr << std::endl;
    }
}

//----------------------------------------------------------------------------//

void setup_comms(MPI_Comm comm) {
    int world_rank, world_size;

    MPI_Comm_rank(comm, &world_rank);
    MPI_Comm_size(comm, &world_size);

    if (world_size % nreplicas != 0 || world_size < int(nreplicas)) {
        if (world_rank == 0)
            std::cerr << " ** Error ** : incommensurate number of threads/replicas: " << world_size << " " << nreplicas
                      << std::endl;

        MPI_Abort(comm, EXIT_FAILURE);
    }

    replica_id = world_rank / (world_size / int(nreplicas));

    MPI_Comm_split(comm, replica_id, world_rank, &comm_replica);

    MPI_Comm_rank(comm_replica, &comm_replica_rank);
    MPI_Comm_size(comm_replica, &comm_replica_size);

    // another communicator for replica masters
    {
        int color = (comm_replica_rank == 0 ? 0 : MPI_UNDEFINED);
        MPI_Comm_split(comm, color, world_rank, &comm_masters);
    }

    if (comm_replica_rank == 0) {
        MPI_Comm_rank(comm_masters, &comm_masters_rank);  // replica_id
        MPI_Comm_size(comm_masters, &comm_masters_size);  // nreplicas
    } else {
        comm_masters_rank = MPI_UNDEFINED;
        comm_masters_size = MPI_UNDEFINED;
    }

    // append suffix if needed
    if (nreplicas > 1) {
        std::ostringstream oss;
        oss << '.' << std::setfill('0') << std::setw(2) << (replica_id + 1);

        initial += oss.str();
        final += oss.str();
        trajectory += oss.str();
        output += oss.str();
        restart += oss.str();
    }
}

//----------------------------------------------------------------------------//

void print_exchange(std::ostream& os, size_t step, const double& Tbefore, const double& Tpartner,
                    const double& Tafter) {
    using namespace std;

    ios::fmtflags saved_flags = os.flags();

    os << fixed << showpoint << setprecision(1) << "# " << setw(8) << step << " EXCH " << setw(6) << Tbefore << ' '
       << setw(6) << Tpartner << ' ' << setw(6) << Tafter << '\n';

    os.flags(saved_flags);
}

////////////////////////////////////////////////////////////////////////////////

my_pimd::my_pimd() : m_virials(0) {}

//----------------------------------------------------------------------------//

my_pimd::~my_pimd() { delete[] m_virials; }

//----------------------------------------------------------------------------//

//
// energies/forces are in the internal units
//

double my_pimd::force(double* pos, double* frc, MPI_Comm comm) {
    using h2o::engunit;

    // for water
    // assert(natom()%3 == 0);

    int comm_rank;
    MPI_Comm_rank(comm, &comm_rank);

    // double Epot = nion::get_E(pos,atnames,natom(),frc);
    std::vector<double> xyz(pos, pos + 3 * natom());
    systems[0].SetRealXyz(xyz);
    double Epot = systems[0].Energy(true);

    std::vector<double> grads = systems[0].GetRealGrads();
    std::copy(grads.begin(), grads.end(), frc);

    double Econ(0);

    if (comm_rank == 0) {
        Econ = m_trap(natom(), m_fict_mass, pos, frc);
        Epot += Econ;

        m_frc_cart[3 * natom() * nbead() + 1] += Econ * engunit;
    }

    for (size_t n = 0; n < 3 * natom(); ++n) frc[n] *= -engunit;

    return engunit * Epot;
}

//----------------------------------------------------------------------------//

void my_pimd::step(const double& dt, MPI_Comm comm) {
    kit::pimd::step(dt, comm);
    m_Econ_sum = m_frc_cart[3 * natom() * nbead() + 1];
}

//----------------------------------------------------------------------------//

void my_pimd::print_energies(std::ostream& os, size_t step) {
    using h2o::engunit;
    using namespace std;

    assert(comm_replica_rank == 0);

    ios::fmtflags saved_flags = os.flags();

    // Econ_sum is included in Epot_sum

    double Ekin_CV(0);

    for (size_t n = 0; n < natom(); ++n) Ekin_CV += m_virials[n];

    // "force virials", hence the '-' below
    Ekin_CV = 1.5 * (natom()) * m_kT / engunit - Ekin_CV;

    os << setprecision(4) << setw(8) << step << ' ' << fixed << showpoint << setw(10) << (step * dt)
       << ' '
       //<< fixed << showpoint << setw(10) << (m_time0 + step*dt) << ' '
       << setprecision(4) << scientific << setw(11) << (m_Epot_sum / engunit) << ' ' << setw(11)
       << (m_Espring / engunit) << ' ' << setw(11) << Ekin_CV << ' ' << setw(11) << (m_Ekin_fict / engunit) << ' '
       << setw(11) << (m_Econ_sum / engunit) << ' ' << setw(11) << (invariant() / engunit) << '\n';

    os.flags(saved_flags);
}

//----------------------------------------------------------------------------//

void my_pimd::set_COM_at_origin() {
    assert(comm_replica_rank == 0);

    double com[3] = {0.0, 0.0, 0.0}, mass_sum(0);

    // centroid's COM
    for (size_t n = 0; n < natom(); ++n) {
        mass_sum += mass(n);
        for (size_t k = 0; k < 3; ++k) com[k] += mass(n) * m_pos_nmode[3 * n + k];
    }

    for (size_t k = 0; k < 3; ++k) com[k] /= mass_sum;

    for (size_t n = 0; n < natom(); ++n)
        for (size_t k = 0; k < 3; ++k) m_pos_nmode[3 * n + k] -= com[k];
}

//----------------------------------------------------------------------------//

void my_pimd::try_exchange(std::ostream& os, size_t step) {
    using h2o::hbar;
    using h2o::kB;

    assert(comm_masters_rank != MPI_UNDEFINED);

    const double random_number = re_prg.random_double();

    // find our partner's rank
    int np = comm_masters_size + comm_masters_size % 2;
    int permutation[np];

    for (int n = 0; n < np; ++n) permutation[n] = n;

    for (int n = 0; n + 1 < np; ++n) {
        int k = n + re_prg() % (np - n);
        std::swap(permutation[n], permutation[k]);
    }

    int partner_rank = MPI_UNDEFINED;

    for (int n = 0; n < np; n += 2)
        if (permutation[n] < comm_masters_size && permutation[n + 1] < comm_masters_size) {
            if (comm_masters_rank == permutation[n]) {
                partner_rank = permutation[n + 1];
                break;
            }
            if (comm_masters_rank == permutation[n + 1]) {
                partner_rank = permutation[n];
                break;
            }
        }

    if (partner_rank == MPI_UNDEFINED) return;

    // get partner's classical potential energy, harmonic energy
    // and temperature; try to make an exchange

    double my[3] = {m_Epot_sum, m_Espring, m_T};
    double his[3];

    MPI_Sendrecv(my, 3, MPI_DOUBLE, partner_rank, 8, his, 3, MPI_DOUBLE, partner_rank, 8, comm_masters,
                 MPI_STATUS_IGNORE);

    int do_exchange = 0;  // don't exchange

    if (comm_masters_rank > partner_rank) {
        const double my_kT = kB * my[2];
        const double his_kT = kB * his[2];

        const double my_beta = 1.0 / my_kT;
        const double his_beta = 1.0 / his_kT;

        const double delta = (his[0] - my[0]) * (my_beta - his_beta) +
                             (his[1] * his_beta * his_beta - my[1] * my_beta * my_beta) * (my_kT - his_kT);

        if (delta < 0.0 || (delta >= 0.0 && std::exp(-delta) > random_number)) do_exchange = 1;

        MPI_Send(&do_exchange, 1, MPI_INT, partner_rank, 9, comm_masters);

    } else
        MPI_Recv(&do_exchange, 1, MPI_INT, partner_rank, 9, comm_masters, MPI_STATUS_IGNORE);

    // complete the exchange if accepted

    if (do_exchange) change_temperature(his[2]);

    print_exchange(os, step, my[2], his[2], m_T);
}

//----------------------------------------------------------------------------//

void my_pimd::compute_virials() {
    using h2o::engunit;

    assert(m_virials != 0);
    assert(comm_replica_rank == 0);

    // cartesian positions/forces are computed/reduced in pimd::step()
    // (cartesian forces do not include srping forces between the beads)

    const size_t natom3 = 3 * natom();
    for (size_t i = 0; i < natom(); ++i) {
        const size_t i3 = 3 * i;
        m_virials[i] = 0.0;
        for (size_t b = 0; b < nbead(); ++b) {
            const size_t j3 = i3 + b * natom3;
            for (size_t k = 0; k < 3; ++k) {
                const double dx = m_pos_cart[j3 + k] - m_pos_nmode[i3 + k];
                m_virials[i] += m_frc_cart[j3 + k] * dx;
            }
        }
        m_virials[i] /= 2 * nbead();
        m_virials[i] /= engunit;
    }
}

//----------------------------------------------------------------------------//

void my_pimd::save_positions(std::ostream& os, size_t step) {
    assert(comm_replica_rank == 0);
    assert(nbead() > 0 && natom() > 0);

    pos_n2c();

    os << kit::io::sec("time", dt * step) << kit::io::sec("temperature", m_T)
       << kit::io::sec("mass", natom(), m_fict_mass) << kit::io::sec("positions", 3 * natom() * nbead(), m_pos_cart)
       << kit::io::endframe();
}

//----------------------------------------------------------------------------//

void my_pimd::save_virials(std::ostream& os, size_t step) {
    assert(comm_replica_rank == 0);
    assert(nbead() > 0 && natom() > 0);

    // os << kit::io::sec("time", m_time0 + dt*step)
    os << kit::io::sec("time", dt * step) << kit::io::sec("temperature", m_T)
       << kit::io::sec("mass", natom(), m_fict_mass) << kit::io::sec("virials", natom(), m_virials)
       << kit::io::endframe();
}

////////////////////////////////////////////////////////////////////////////////

std::string my_pimd::mass2name(double mass) {
    std::string name;
    if (mass < 0) {
        std::cerr << "Negative mass not possible" << std::endl;
        throw std::exception();
    } else if (mass < 1.01) {
        name = "H";
    } else if (mass < 2.02) {
        name = "D";
    } else if (mass < 7.02) {
        name = "Li";
    } else if (mass < 16.00) {
        name = "O";
    } else if (mass < 19.00) {
        name = "F";
    } else if (mass < 23.00) {
        name = "Na";
    } else if (mass < 35.00) {
        name = "Cl";
    } else if (mass < 39.0) {
        name = "K";
    } else if (mass < 79.00) {
        name = "Br";
    } else if (mass < 85.00) {
        name = "Rb";
    } else if (mass < 127.00) {
        name = "I";
    } else if (mass < 133.00) {
        name = "Cs";
    } else {
        std::cerr << "Mass " << mass << " not defined." << std::endl;
        throw std::exception();
    }
    return name;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////
