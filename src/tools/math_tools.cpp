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

#include "math_tools.h"

std::vector<double> InvertUnitCell(const std::vector<double> &box) {
    std::vector<double> box_inverse(9);
    if (box.size() != 9) {
        box_inverse.clear();
        return box_inverse;
    }

    double determinant = box[0] * (box[4] * box[8] - box[7] * box[5]) - box[1] * (box[3] * box[8] - box[5] * box[6]) +
                         box[2] * (box[3] * box[7] - box[4] * box[6]);

    // Check matrix has inverse
    if (std::fabs(determinant) < 1E-36) {
        std::string text = "Matrix has no inverse. Determinant is 0 (" + std::to_string(determinant) + "}..\n";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    double determinant_inverse = 1 / determinant;
    box_inverse[0] = (box[4] * box[8] - box[7] * box[5]) * determinant_inverse;
    box_inverse[1] = (box[2] * box[7] - box[1] * box[8]) * determinant_inverse;
    box_inverse[2] = (box[1] * box[5] - box[2] * box[4]) * determinant_inverse;
    box_inverse[3] = (box[5] * box[6] - box[3] * box[8]) * determinant_inverse;
    box_inverse[4] = (box[0] * box[8] - box[2] * box[6]) * determinant_inverse;
    box_inverse[5] = (box[3] * box[2] - box[0] * box[5]) * determinant_inverse;
    box_inverse[6] = (box[3] * box[7] - box[6] * box[4]) * determinant_inverse;
    box_inverse[7] = (box[6] * box[1] - box[0] * box[7]) * determinant_inverse;
    box_inverse[8] = (box[0] * box[4] - box[3] * box[1]) * determinant_inverse;
    return box_inverse;
}

double switch_function(const double &r, const double &ri, const double &ro, double &g) {
    if (r > ro) {
        g = 0.0;
        return 0.0;
    } else if (r > ri) {
        const double t1 = M_PI / (ro - ri);
        const double x = (r - ri) * t1;
        g = -std::sin(x) * t1 / 2.0;
        return (1.0 + std::cos(x)) / 2.0;
    } else {
        g = 0.0;
        return 1.0;
    }
}

bool IsZero(double a, double t) { return a * a < t * t; }

std::vector<double> BoxVecToBoxABCabc(std::vector<double> box) {
    double A, B, C, alpha, beta, gamma;

    // Check that first vector (3 first elements of the box) is only in the x axis
    if (IsZero(box[0])) {
        std::string text =
            "X component of first vector in box cannot be 0. Please double check your box definition.";  // +
                                                                                                         // std::to_string(nmax)
                                                                                                         // + " is not
                                                                                                         // acceptable.
                                                                                                         // Possible
                                                                                                         // values are 2
                                                                                                         // or 3.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    if (!IsZero(box[1]) or !IsZero(box[2])) {
        std::string text =
            "Y and Z components of first vector in box must be 0. Please double check your box definition.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Check that y component of second vector is not 0
    if (IsZero(box[4])) {
        std::string text = "Y component of second vector in box cannot be 0. Please double check your box definition.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Check that second vector is in XY plane
    if (!IsZero(box[5])) {
        std::string text = "Z component of second vector in box must be 0. Please double check your box definition.";
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    A = box[0];
    B = sqrt(box[3] * box[3] + box[4] * box[4]);
    C = sqrt(box[6] * box[6] + box[7] * box[7] + box[8] * box[8]);

    double AdotB = box[0] * box[3];
    double AdotC = box[0] * box[6];
    double BdotC = box[3] * box[6] + box[4] * box[7];

    gamma = acos(AdotB / A / B);
    beta = acos(AdotC / A / C);
    alpha = acos(BdotC / B / C);

    std::vector<double> box_out = {A, B, C, alpha / M_PI * 180.0, beta / M_PI * 180.0, gamma / M_PI * 180.0};
    return box_out;
}

std::vector<double> BoxABCabcToBoxVec(std::vector<double> box) {
    double A, B, C, alpha, beta, gamma;
    A = box[0];
    B = box[1];
    C = box[2];
    alpha = box[3] / 180.0 * M_PI;
    beta = box[4] / 180.0 * M_PI;
    gamma = box[5] / 180.0 * M_PI;

    std::vector<double> box_out(9, 0.0);
    box_out[0] = A;
    box_out[3] = B * cos(gamma);
    box_out[4] = B * sin(gamma);
    box_out[6] = C * cos(beta);
    double tmp = (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma);
    box_out[7] = C * tmp;
    box_out[8] = C * sqrt(1.0 - cos(beta) * cos(beta) - tmp * tmp);

    return box_out;
}
