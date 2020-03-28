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

#ifdef FIX_CLASS

FixStyle(mbx,FixMBX)

#else

#ifndef LMP_FIX_MBX_H
#define LMP_FIX_MBX_H

#include "fix.h"

// MBX

#include "bblock/system.h"

enum {
      MBXT_INIT=0,
      MBXT_UPDATE_XYZ,
      MBXT_INIT_FULL,
      MBXT_UPDATE_XYZ_FULL,
      MBXT_E1B,
      MBXT_E2B_LOCAL,
      MBXT_E2B_GHOST,
      MBXT_E3B_LOCAL,
      MBXT_E3B_GHOST,
      MBXT_DISP,
      MBXT_DISP_PME,
      MBXT_BUCK,
      MBXT_ELE,
      MBXT_ACCUMULATE_F,
      MBXT_ACCUMULATE_F_FULL,

      MBXT_NUM_TIMERS
};

namespace LAMMPS_NS {

class FixMBX : public Fix {
  friend class PairMBX;
  
 public:
  FixMBX(class LAMMPS *, int, char **);
  ~FixMBX();
  int setmask();
  virtual void post_constructor();
  virtual void init();
  virtual void init_storage();
  virtual void setup_post_neighbor();
  virtual void post_neighbor();
  void setup_pre_force(int);
  virtual void pre_force(int);

  void min_setup_pre_force(int);
  void min_pre_force(int);

 protected:
  class PairMBX * pair_mbx; // pointer to MBX pair_style
  
  bblock::System * ptr_mbx;       // pointer to MBX object
  bblock::System * ptr_mbx_full;  // pointer to MBX object for full system
  bblock::System * ptr_mbx_local; // pointer to MBX object for local atoms
  bblock::System * ptr_mbx_pme;   // pointer to MBX object for full system MPI-enabled

  int me, nprocs;
  bigint ngroup;

  bool mbx_mpi_enabled;
  bool mbx_write_warnings;
  
  int num_mol_types;       // # of unique molecule types
  int num_molecules;       // total # of molecules
  int * num_mols;          // array of # of molecules of each type
  int * num_atoms_per_mol; // array of # of atoms per molecule for each type
  tagint * mol_offset;     // array of atom count per molecule prefix sum
  char ** mol_names;       // array of molecule names 

  int * mol_type;     // per-atom array of molecule type
  int * mol_anchor;   // per-atom array 1/0 if anchor atom of a molecule
  int * mol_local;    // per-molecule array 1/0 if molecule has at least one local particle

  int use_json;
  char * json_file;
  std::string json_settings;
  
  int mbx_num_atoms;
  int mbx_num_atoms_full;
  int mbx_num_atoms_local;
  int mbx_num_atoms_pme;

  int * mbxt_count;
  double * mbxt_time;
  double * mbxt_time_start;
  double mbxt_initial_time;
  void mbxt_start(int);
  void mbxt_stop(int);
  void mbxt_write_summary();
  void mbxt_print_time(const char *, int, double *);
  
  // rank 0's copy of all atoms in simulation cell
  
  int * mol_anchor_full;
  int * mol_type_full;
  double ** x_full;
  double ** f_full;
  double ** f_pme;
  double ** f_local;
  tagint * tag_full;
  int * atom_map_full;
  int * nlocal_rank;
  int * nlocal_disp;
  int * nlocal_rank3;
  int * nlocal_disp3;
  
  void mbx_init();
  void mbx_init_full();
  //  void mbx_init_local();
  void mbx_init_pme();
  
  void mbx_update_xyz();
  void mbx_update_xyz_full();
  //  void mbx_update_xyz_local();
  void mbx_update_xyz_pme();
  
  virtual int pack_forward_comm(int, int *, double *, int, int *);
  virtual void unpack_forward_comm(int, int, double *);
  virtual int pack_reverse_comm(int, int, double *);
  virtual void unpack_reverse_comm(int, int *, double *);
  virtual double memory_usage();
  virtual void grow_arrays(int);
  virtual void copy_arrays(int, int, int);
  virtual int pack_exchange(int, double *);
  virtual int unpack_exchange(int, double *);
};

}

#endif
#endif
