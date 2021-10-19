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

#include "tools/properties.h"

namespace tools {

double GetMass(const std::string atom) {
    // The masses are the ones from the most abundant isotope
    // Taken from NIST on November 7th 2015 by Marc R.
    // http://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl?ele=&all=all&ascii=html
    double mass = 0.0;
    if (atom == "H") {
        mass = 1.007825032;
    } else if (atom == "D") {
        mass = 2.01410178;
    } else if (atom == "He") {
        mass = 4.00260325413;
    } else if (atom == "C") {
        mass = 12.0000000;
    } else if (atom == "N") {
        mass = 14.0030740044;
    } else if (atom == "O") {
        mass = 15.994914619;
    } else if (atom == "F") {
        mass = 18.998403162;
    } else if (atom == "Cl") {
        mass = 34.968852682;
    } else if (atom == "Ar") {
        mass = 39.9623831237;
    } else if (atom == "Br") {
        mass = 78.9183376;
    } else if (atom == "I") {
        mass = 126.9044719;
    } else if (atom == "Li") {
        mass = 7.0160034366;
    } else if (atom == "Na") {
        mass = 22.989769282;
    } else if (atom == "K") {
        mass = 38.9637064864;
    } else if (atom == "Rb") {
        mass = 84.9114977282;
    } else if (atom == "Cs") {
        mass = 132.905429;
    } else if (atom == "X") {
        mass = 0.0;
    } else {
        std::string text = "Atom " + atom + " not found in database.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    return mass;
}

double Distance(double *p1, double *p2, std::vector<double> box, std::vector<double> box_inverse) {
    // Get distance
    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double dz = p2[2] - p1[2];

    // Check if PBC needs to be used
    if (box.size()) {
        // Convert to fractional coordinates
        double dxf = box_inverse[0] * dx + box_inverse[3] * dy + box_inverse[6] * dz;
        double dyf = box_inverse[1] * dx + box_inverse[4] * dy + box_inverse[7] * dz;
        double dzf = box_inverse[2] * dx + box_inverse[5] * dy + box_inverse[8] * dz;

        // Put in the range 0-1
        dxf -= std::floor(dxf + 0.5);
        dyf -= std::floor(dyf + 0.5);
        dzf -= std::floor(dzf + 0.5);

        // Convert back to cartesian coordinates
        dx = box[0] * dxf + box[3] * dyf + box[6] * dzf;
        dy = box[1] * dxf + box[4] * dyf + box[7] * dzf;
        dz = box[2] * dxf + box[5] * dyf + box[8] * dzf;
    }

    double d = std::sqrt(dx * dx + dy * dy + dz * dz);
    return d;
}

}  // namespace tools
