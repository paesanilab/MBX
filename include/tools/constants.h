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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>

namespace constants {

const double Eh_J = 4.35974434e-18;  // CODATA 2010
const double Na = 6.02214129e+23;    // CODATA 2010

const double kcal_J = 4184.0;
const double Eh_kcalmol = Eh_J * Na / kcal_J;

const double Bohr_A = 0.52917721092;  // CODATA 2010

const double c0 = 299792458.0;      // m/s CODATA 2010
const double ea0 = 8.47835326e-30;  // C*m CODATA 2010

const double D_au = (1.0 / c0) * 1.0e-21 / ea0;  // e * Bohr
const double D = D_au * Bohr_A;                  // e * A

const double h_Js = 6.62606957e-34;                 // J*s CODATA 2010
const double hbar_Js = 1.054571726e-34;             // J*s CODATA 2010
const double Eh_cm1 = 1.0e-2 * Eh_J / (c0 * h_Js);  // cm-1

const double cm1_kcalmol = Eh_kcalmol / Eh_cm1;

const double kB = 1.3806488e-23;  // JK-1 CODATA 2010

const double DEBYE = 1.0 / 0.20822678;
const double au2debye = Bohr_A / 0.20822678;

// Numeric limits
const double EPS = std::numeric_limits<double>::epsilon();
const double max_dbl = std::numeric_limits<double>::max() * EPS;

namespace details {

const double e = 1.602176565e-19;  // C CODATA 2010

// interaction energy of 2 unit charges 1A apart
const double E_cc = 1.0e-7 * (c0 * e * c0 * e) / 1.0e-10;  // in J

}  // namespace details

const double COULOMB = details::E_cc * Na / kcal_J;
const double CHARGECON = std::sqrt(COULOMB);

// from NIST web site

const double H_mass = 1.00782503207;
const double O_mass = 15.99491461956;

}  // namespace constants

namespace h2o {

const double O_mass = 15.9949;
const double H_mass = 1.0079;

// internal units are DLPOLY2

const double engunit = 418.4;      // conversion from internal units to kcal/mol
const double kB = 8.31451115e-01;  // Boltzmann constant in internal units
const double hbar = 6.350780668;   // in internal units (not 2010 value)

}  // namespace h2o
#endif  // CONSTANTS_H
