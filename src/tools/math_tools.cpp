#include "math_tools.h"

std::vector<double> InvertUnitCell(const std::vector<double> &box) {
    double determinant = box[0] * (box[4] * box[8] - box[7] * box[5]) - box[1] * (box[3] * box[8] - box[5] * box[6]) +
                         box[2] * (box[3] * box[7] - box[4] * box[6]);

    double determinant_inverse = 1 / determinant;
    std::vector<double> box_inverse(9);
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
