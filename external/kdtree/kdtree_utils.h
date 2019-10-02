/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2011-2016 Jose Luis Blanco (joseluisblancoc@gmail.com).
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

/*************************************************************************
 * This software was modified by Marc Riera (mrierari@ucsd,edu) to be
 * adapted to our software.
 ************************************************************************/

#ifndef KDTREE_UTILS_H
#define KDTREE_UTILS_H

#include "kdtree/nanoflann.hpp"

namespace kdtutils {

template <typename T>
struct PointCloud {
    struct Point {
        T x, y, z;
    };

    std::vector<Point> pts;

    // Sets if PBC will be used
    bool PBC = false;

    // Box. Assuming orthogonal box. 3 components in vector
    std::vector<T> pbcbox;

    // Must return the number of data points
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    // Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the
    // class:
    inline T kdtree_distance(const T *p1, const size_t idx_p2, size_t size) const {
        //    T tmp, d;
        //    if (PBC) {
        //      tmp = p1[0]-pts[idx_p2].x;
        //      const T d0 = tmp >  pbcbox[0] * 0.5 ? tmp - pbcbox[0] * 0.5 :
        //                   tmp <= -pbcbox[0] * 0.5 ? tmp + pbcbox[0] * 0.5 : tmp;
        //      tmp = p1[1]-pts[idx_p2].y;
        //      const T d1 = tmp >  pbcbox[1] * 0.5 ? tmp - pbcbox[1] * 0.5 :
        //                   tmp <= -pbcbox[1] * 0.5 ? tmp + pbcbox[1] * 0.5 : tmp;
        //      tmp = p1[2]-pts[idx_p2].z;
        //      const T d2 = tmp >  pbcbox[2] * 0.5 ? tmp - pbcbox[2] * 0.5 :
        //                   tmp <= -pbcbox[2] * 0.5 ? tmp + pbcbox[2] * 0.5 : tmp;
        //      d = d0*d0+d1*d1+d2*d2;
        //    } else {
        const T d0 = p1[0] - pts[idx_p2].x;
        const T d1 = p1[1] - pts[idx_p2].y;
        const T d2 = p1[2] - pts[idx_p2].z;
        T d = d0 * d0 + d1 * d1 + d2 * d2;
        //    }
        return d;
    }

    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate value, the
    //  "if/else's" are actually solved at compile time.
    inline T kdtree_get_pt(const size_t idx, int dim) const {
        if (dim == 0)
            return pts[idx].x;
        else if (dim == 1)
            return pts[idx].y;
        else
            return pts[idx].z;
    }

    // Optional bounding-box computation: return false to default to a standard bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it
    //   again. Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX & /*bb*/) const {
        return false;
    }
};

// Gets a vector of XYZ in vectorized form and returns the point cloud
// All the coordinates will be modified to be the closest image to the reference point
template <typename T>
PointCloud<T> XyzToCloudVec(std::vector<T> &xyz, size_t npoints, std::vector<T> box, std::vector<T> reference) {
    PointCloud<T> ptc;
    ptc.PBC = box.size();
    ptc.pbcbox = box;
    ptc.pts.resize(npoints);

    std::vector<T> box2 = box;
    for (size_t i = 0; i < box.size(); i++) box2[i] *= 0.5;

    for (size_t dim = 0; dim < 3; dim++) {
        size_t dimnp = dim * npoints;
        for (size_t i = 0; i < npoints; i++) {
            T coord = xyz[dimnp + i];
            T di = coord - reference[dim];
            // NOTE Assuming orthorombic box for now
            if (di > box2[3 * dim + dim]) {
                // here
                xyz[dimnp + i] -= box[3 * dim + dim];
                // here
            } else if (di <= -box2[3 * dim + dim]) {
                // here
                xyz[dimnp + i] += box[3 * dim + dim];
            }
        }
    }

    for (size_t i = 0; i < npoints; i++) {
        ptc.pts[i].x = xyz[i];
        ptc.pts[i].y = xyz[i + npoints];
        ptc.pts[i].z = xyz[i + 2 * npoints];
    }

    return ptc;
}

// Gets a vector of XYZ and returns the point cloud
template <typename T>
PointCloud<T> XyzToCloud(std::vector<T> xyz, bool use_pbc, std::vector<T> box) {
    PointCloud<T> ptc;
    ptc.PBC = use_pbc;
    ptc.pbcbox = box;

    size_t np = xyz.size() / 3;
    size_t size = np;
    if (use_pbc) size *= 27;

    ptc.pts.resize(size);

    std::vector<int> indxs = {0, -1, 1};

    for (size_t i = 0; i < np; i++) {
        size_t i3 = 3 * i;
        ptc.pts[i].x = xyz[i3];
        ptc.pts[i].y = xyz[i3 + 1];
        ptc.pts[i].z = xyz[i3 + 2];
        if (use_pbc) {
            for (size_t m2 = 0; m2 < 3; m2++) {
                for (size_t n2 = 0; n2 < 3; n2++) {
                    for (size_t l2 = 0; l2 < 3; l2++) {
                        if (m2 == 0 && n2 == 0 && l2 == 0) continue;
                        int m = indxs[m2];
                        int n = indxs[n2];
                        int l = indxs[l2];
                        size_t shift = (9 * m2 + 3 * n2 + l2) * np;
                        std::vector<T> shifti = {T(m) * box[0] + T(n) * box[3] + T(l) * box[6],
                                                 T(m) * box[1] + T(n) * box[4] + T(l) * box[7],
                                                 T(m) * box[2] + T(n) * box[5] + T(l) * box[8]};

                        //            std::vector<double> shifti = {(double(m) - 1) * box[0],
                        //                                          (double(n) - 1) * box[1],
                        //                                          (double(l) - 1) * box[2]};
                        ptc.pts[i + shift].x = xyz[i3] + shifti[0];
                        ptc.pts[i + shift].y = xyz[i3 + 1] + shifti[1];
                        ptc.pts[i + shift].z = xyz[i3 + 2] + shifti[2];
                    }
                }
            }
        }
    }
    return ptc;
}

}  // namespace kdtutils

#endif  // KDTREE_UTILS_H
