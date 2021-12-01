#include "pbctools.h"

namespace tools {

void GetClosePoint(std::vector<double> p1, std::vector<double> &p2, std::vector<double> box,
                   std::vector<double> box_inv) {
    double x_rec = box_inv[0] * p1[0] + box_inv[3] * p1[1] + box_inv[6] * p1[2];
    double y_rec = box_inv[1] * p1[0] + box_inv[4] * p1[1] + box_inv[7] * p1[2];
    double z_rec = box_inv[2] * p1[0] + box_inv[5] * p1[1] + box_inv[8] * p1[2];

    double xr = box_inv[0] * p2[0] + box_inv[3] * p2[1] + box_inv[6] * p2[2];
    double yr = box_inv[1] * p2[0] + box_inv[4] * p2[1] + box_inv[7] * p2[2];
    double zr = box_inv[2] * p2[0] + box_inv[5] * p2[1] + box_inv[8] * p2[2];

    xr -= std::floor(xr - x_rec + 0.5);
    yr -= std::floor(yr - y_rec + 0.5);
    zr -= std::floor(zr - z_rec + 0.5);

    p2[0] = box[0] * xr + box[3] * yr + box[6] * zr;
    p2[1] = box[1] * xr + box[4] * yr + box[7] * zr;
    p2[2] = box[2] * xr + box[5] * yr + box[8] * zr;
}

}  // namespace tools
