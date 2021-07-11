#ifndef RMSD_H
#define RMSD_H

#include <cstddef>

namespace rmsd_tools {

// puts x? COMs to origin
double rmsd(size_t n, const double* w,  // weights (n)
            double* x1,                 // coordinates #1 (3*n)
            double* x2);                // coordinates #2 (3*n)

// finds optimal rotation
void rmsd_q(size_t n, const double* w,  // weights (n)
            const double* x1,           // coordinates #1 (3*n)
            const double* x2,           // coordinates #2 (3*n)
            double& lambda,             // eigenvalue
            double q[4] /* quaternion */);

// constructs rotation matrix U from quaternion q
void rmsd_q_to_rotation(const double q[4], double* U);

}  // namespace rmsd_tools

#endif  // RMSD_H
