/* Original code from Greg Medders. Adapted by Marc Riera in 20171005*/

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <omp.h>

namespace {
const double step_size = 0.2;    // Angstrom
const double kB = 0.0019872041;  // kcal/mol/K from wiki
const double PITIMES2 = 2 * M_PI;
const double max_dist = 20;  // Angstrom
double emin = 0.0;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

double mayer(double e, double T) { return std::exp(-e / kB / T) - 1.0; }

////////////////////////////////////////////////////////////////////////////////

void average_mayer_function(std::vector<std::vector<double> > energies, std::vector<double> angles,
                            std::vector<double> temps, std::vector<double>& M3T0, std::vector<double>& M3T3B) {
    size_t ns = energies.size();

    for (size_t i = 0; i < ns; i++) {
        double sin_ang = sin(angles[i]);
        double phi12 = -1;
        double phi13 = -1;
        double phi23 = -1;

        for (size_t j = 0; j < temps.size(); j++) {
            if (energies[i][1] > emin) {
                phi12 = mayer(energies[i][1], temps[j]);
            }

            if (energies[i][2] > emin) {
                phi13 = mayer(energies[i][2], temps[j]);
            }

            if (energies[i][3] > emin) {
                phi23 = mayer(energies[i][3], temps[j]);
            }

            M3T0[j] += sin_ang * phi12 * phi13 * phi23;

            double em3t3b = (phi12 + 1) * (phi13 + 1) * (phi23 + 1);

            if (em3t3b > 1E-30 && energies[i][4] > 2 * emin) {
                M3T3B[j] += sin_ang * mayer(energies[i][4], temps[j]) * em3t3b;
            }
        }
    }

    for (size_t j = 0; j < temps.size(); j++) {
        M3T0[j] /= double(ns);
        M3T3B[j] /= double(ns);
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <Temperature> <E_min_trimer> <index-distance_file" << std::endl;
        std::cout << "If T < 0, will use the default temperatures:\n "
                  << " {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400,\n"
                  << " 450, 500, 550, 600, 650, 700, 750, 800 900 1000 \n"
                  << " 1100 1200 1400 1600 1800}";
        return 1;
    }

    // 1. Load Configurations
    const double dR = step_size;

    std::string virial_output_file(argv[3]);
    virial_output_file = "virial_data_" + virial_output_file;

    const double T = atof(argv[1]);
    std::vector<double> temps;
    if (T < 0) {
        temps = {100, 125, 150, 175, 200, 225, 250, 275, 300,  325,  350,  375,  400,  450,
                 500, 550, 600, 650, 700, 750, 800, 900, 1000, 1100, 1200, 1400, 1600, 1800};
    } else {
        temps.push_back(T);
    }

    int ns = 0;
    emin = atof(argv[2]);

    // 2. Do trapezoidal rule for numerical integration of radial coord
    //    Don't start at R = 0 because first (and final) points are zero
    //    We do a double loop looping from distance 1-2 and 1-3

    // First read the index distance file and store index, r12 and r13
    std::vector<size_t> indexes;
    std::vector<double> d12;
    std::vector<double> d13;

    std::ifstream ifs(argv[3]);
    while (true) {
        size_t idx;
        double ir12;
        double ir13;
        ifs >> idx >> ir12 >> ir13;
        if (!ifs.good()) break;

        indexes.push_back(idx);
        d12.push_back(ir12);
        d13.push_back(ir13);
    }

    int rank = 0;
    int nthreads = 1;

#pragma omp parallel
    {
        rank = omp_get_thread_num();
        if (rank == 0) nthreads = omp_get_num_threads();
    }

    // Declare mayer pools
    std::vector<double> B3T0(temps.size(), 0.0);
    std::vector<double> B3T3B(temps.size(), 0.0);
    std::vector<std::vector<double> > M3T0_pool(nthreads, std::vector<double>(temps.size(), 0.0));
    std::vector<std::vector<double> > M3T3B_pool(nthreads, std::vector<double>(temps.size(), 0.0));

    // Loop over each index
#pragma omp parallel for schedule(dynamic) private(rank)
    for (size_t i = 0; i < indexes.size(); i++) {
        rank = omp_get_thread_num();
        // Retrieve energies and angles
        std::vector<std::vector<double> > energies;
        std::vector<double> angles;

        std::string energy_file = std::to_string(indexes[i]) + ".dat";
        std::string angle_file = std::to_string(indexes[i]) + ".angles";

        std::ifstream ifsen(energy_file.c_str());
        std::ifstream ifsan(angle_file.c_str());

        // Both angle and energy files exist and are ok
        // Read files
        std::vector<double> e(5, 0.0);
        double a = 0.0;
        while (true) {
            ifsen >> e[0] >> e[1] >> e[2] >> e[3] >> e[4];
            ifsan >> a;
            if (!ifsen.good()) break;
            energies.push_back(e);
            angles.push_back(a);
        }

        std::vector<double> m3t0(temps.size(), 0.0);
        std::vector<double> m3t3b(temps.size(), 0.0);

        // Calculate the average mayer function for index
        average_mayer_function(energies, angles, temps, m3t0, m3t3b);

        // Add it to the pool
        for (size_t j = 0; j < temps.size(); j++) {
            M3T0_pool[rank][j] += m3t0[j] * d12[i] * d12[i] * d13[i] * d13[i];
            M3T3B_pool[rank][j] += m3t3b[j] * d12[i] * d12[i] * d13[i] * d13[i];
        }
    }

    // Condense the virial contributions
    for (size_t i = 0; i < nthreads; i++) {
        for (size_t j = 0; j < temps.size(); j++) {
            B3T0[j] += M3T0_pool[i][j];
            B3T3B[j] += M3T3B_pool[i][j];
        }
    }

    // Add missing terms and convert to cm6/mol2 : * 6.022 E+23 / 1 E+24 * 6.022 E+23 / 1 E+24
    for (size_t i = 0; i < temps.size(); i++) {
        // Multipli by -8/3 * pi^2 * dr12 * dr13 * pi ; last pi is for integral of sin
        B3T0[i] *= -dR * dR * PITIMES2 * PITIMES2 * PITIMES2 / 3.0;
        B3T3B[i] *= -dR * dR * PITIMES2 * PITIMES2 * PITIMES2 / 3.0;
        // Convert to cm6/mol2
        B3T0[i] *= 0.6022 * 0.6022;
        B3T3B[i] *= 0.6022 * 0.6022;
    }

    std::ofstream off(virial_output_file.c_str());

    for (size_t i = 0; i < temps.size(); i++) {
        off << std::scientific << std::setprecision(12) << std::setw(20) << temps[i] << std::setw(20) << B3T0[i]
            << std::setw(20) << B3T3B[i] << std::setw(20) << B3T0[i] + B3T3B[i] << std::endl;
    }

    off.close();

    return 0;
}
