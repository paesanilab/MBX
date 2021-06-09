/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifdef PAIR_CLASS

PairStyle(mbx, PairMBX)

#else

#ifndef LMP_PAIR_MBX_H
#define LMP_PAIR_MBX_H

#include "pair.h"
#include "fix_mbx.h"

// MBX

#include "bblock/system.h"

namespace LAMMPS_NS {

class PairMBX : public Pair {
    friend FixMBX;  // accesses cut_global

   public:
    PairMBX(class LAMMPS *);
    virtual ~PairMBX();
    virtual void compute(int, int);
    void settings(int, char **);
    void coeff(int, char **);
    void init_style();
    double init_one(int, int);
    void write_data(FILE *);
    void write_data_all(FILE *);
    void *extract(const char *, int &);

   protected:
    double cut_global;
    double **cut;

    int me;

    FixMBX *fix_mbx;  // owner of MBX objects

    int nmolecule;  // # of molecules in system (would break if number of molecules can change)

    double mbx_e1b;
    double mbx_e2b;
    double mbx_e3b;
    double mbx_disp;
    double mbx_buck;
    double mbx_ele;
    double mbx_total_energy;

    double mbx_virial[6];

    virtual void allocate();
    void setup();

    void accumulate_f();
    void accumulate_f_full();
    void accumulate_f_local();
};

}  // namespace LAMMPS_NS

#endif
#endif

    /* ERROR/WARNING messages:

    E: Illegal ... command

    Self-explanatory.  Check the input script syntax and compare to the
    documentation for the command.  You can use -echo screen as a
    command-line option when running LAMMPS to see the offending line.

    E: Incorrect args for pair coefficients

    Self-explanatory.  Check the input script or data file.

    E: Pair cutoff < Respa interior cutoff

    One or more pairwise cutoffs are too short to use with the specified
    rRESPA cutoffs.

    */
