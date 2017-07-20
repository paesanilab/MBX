#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {

const double Eh_J = 4.35974434e-18; // CODATA 2010
const double Na = 6.02214129e+23; // CODATA 2010

const double kcal_J = 4184.0;
const double Eh_kcalmol = Eh_J*Na/kcal_J;

const double Bohr_A = 0.52917721092; // CODATA 2010

const double c0 = 299792458.0; // m/s CODATA 2010
const double ea0 = 8.47835326e-30; // C*m CODATA 2010

const double D_au = (1.0/c0)*1.0e-21/ea0; // e * Bohr
const double D = D_au*Bohr_A; // e * A

const double h_Js = 6.62606957e-34; // J*s CODATA 2010
const double hbar_Js = 1.054571726e-34; // J*s CODATA 2010
const double Eh_cm1 = 1.0e-2*Eh_J/(c0*h_Js); // cm-1

const double cm1_kcalmol = Eh_kcalmol/Eh_cm1;

const double kB = 1.3806488e-23; // JK-1 CODATA 2010

const double DEBYE = 1.0/0.20822678;
const double au2debye = Bohr_A/0.20822678;

namespace details {

const double e =  1.602176565e-19; // C CODATA 2010

// interaction energy of 2 unit charges 1A apart
const double E_cc = 1.0e-7*(c0*e*c0*e)/1.0e-10; // in J

} // namespace details

const double CHARGECON = std::sqrt(details::E_cc*Na/kcal_J);

// from NIST web site

const double H_mass = 1.00782503207;
const double O_mass = 15.99491461956;

} // namespace constants

namespace h2o {

const double O_mass = 15.9949;
const double H_mass = 1.0079;

// internal units are DLPOLY2

const double engunit = 418.4; // conversion from internal units to kcal/mol
const double kB = 8.31451115e-01; // Boltzmann constant in internal units
const double hbar = 6.350780668; // in internal units (not 2010 value)

} // namespace h2o
#endif // CONSTANTS_H
