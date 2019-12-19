/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "fix_mbx.h"
#include <mpi.h>
#include <cmath>
#include <cstring>
#include "pair_mbx.h"
#include "atom.h"
#include "comm.h"
#include "domain.h"
#include "neighbor.h"
#include "neigh_list.h"
#include "neigh_request.h"
#include "update.h"
#include "force.h"
#include "group.h"
#include "pair.h"
#include "respa.h"
#include "memory.h"
#include "citeme.h"
#include "error.h"

#define _MAX_SIZE_MOL_NAME 10

using namespace LAMMPS_NS;
using namespace FixConst;

static const char cite_fix_mbx[] =
  "fix mbx command:\n\n"
  "@Article{,\n"
  " author = {},\n"
  " title = {},\n"
  " journal = {},\n"
  " year =    ,\n"
  " volume =  ,\n"
  " pages =   {}\n"
  "}\n\n";

/* ---------------------------------------------------------------------- */

FixMBX::FixMBX(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (lmp->citeme) lmp->citeme->add(cite_fix_mbx);

  me = comm->me;
  
  if (narg<5) error->all(FLERR,"Illegal fix mbx command");

  num_mol_types = force->inumeric(FLERR,arg[3]);
  
  if (num_mol_types < 1) error->all(FLERR,"Illegal fix mbx command");

  num_mols  = NULL;
  num_atoms_per_mol = NULL;
  mol_names = NULL;
  
  memory->create(num_mols, num_mol_types, "fixmbx:num_mols");
  memory->create(num_atoms_per_mol, num_mol_types, "fixmbx:num_atoms_per_mol");
  memory->create(mol_names, num_mol_types, _MAX_SIZE_MOL_NAME, "fixmbx:mol_names");

  int iarg = 4;
  for(int i=0; i<num_mol_types; ++i) {
    num_mols[i] = force->inumeric(FLERR,arg[iarg++]);
    strcpy(mol_names[i], arg[iarg++]);
  }
  
  mol_offset = NULL;
  memory->create(mol_offset, num_mol_types+1, "fixmbx:mol_offset");

  // assign # of atoms per molecule based on molecule name
  // -- use this as first pass whether molecule supported by MBX

  for(int i=0; i<num_mol_types; ++i) {

    if(      strcmp("h2o", mol_names[i]) == 0) num_atoms_per_mol[i] = 3;
    else if (strcmp("na",  mol_names[i]) == 0) num_atoms_per_mol[i] = 1;
    else if (strcmp("cl",  mol_names[i]) == 0) num_atoms_per_mol[i] = 1;
    else if (strcmp("co2", mol_names[i]) == 0) num_atoms_per_mol[i] = 3;
    else error->all(FLERR,"Unsupported molecule type in MBX");
  }
  
  mol_offset[0] = 0;
  mol_offset[1] = num_mols[0]*num_atoms_per_mol[0];
  for(int i=1; i<num_mol_types; ++i)
    mol_offset[i+1] = mol_offset[i] + num_mols[i]*num_atoms_per_mol[i];

  printf("\n[MBX] # molecule types= %i\n",num_mol_types);
  for(int i=0; i<num_mol_types; ++i)
    printf("[MBX]   i= %i  # of molecules= %i  name= '%s'  offset= %i\n",i,num_mols[i],mol_names[i],mol_offset[i]);
  printf("\n");

  pair_mbx = NULL;
  pair_mbx = (PairMBX *) force->pair_match("^mbx",0);

  ptr_mbx = NULL;
  ptr_mbx_full = NULL;
  
  mol_type = NULL;
  mol_anchor = NULL;
  grow_arrays(atom->nmax);
  atom->add_callback(0);
  atom->add_callback(1);
}

/* ---------------------------------------------------------------------- */

FixMBX::~FixMBX()
{
  if (copymode) return;
  
  memory->destroy(mol_offset);
  memory->destroy(mol_names);
  memory->destroy(num_mols);
  
  // unregister callbacks to this fix from Atom class

  atom->delete_callback(id,0);
  atom->delete_callback(id,1);
  
  memory->destroy(mol_type);
  memory->destroy(mol_anchor);

  if(ptr_mbx) delete ptr_mbx;
  if(ptr_mbx_full) delete ptr_mbx_full;
}

/* ---------------------------------------------------------------------- */

void FixMBX::post_constructor()
{

}

/* ---------------------------------------------------------------------- */

int FixMBX::setmask()
{
  int mask = 0;
  mask |= POST_NEIGHBOR;
  mask |= PRE_FORCE;
  mask |= PRE_FORCE_RESPA;
  mask |= MIN_PRE_FORCE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixMBX::init()
{
  if (!atom->q_flag)
    error->all(FLERR,"Fix qeq/reax requires atom attribute q");

  ngroup = group->count(igroup);
  if (ngroup == 0) error->all(FLERR,"Fix mbx group has no atoms");

  // I don't think we need neighbor lists yet...
  
  // need a half neighbor list w/ Newton off
  // built whenever re-neighboring occurs

  int irequest = neighbor->request(this,instance_me);
  neighbor->requests[irequest]->pair = 0;
  neighbor->requests[irequest]->fix = 1;
  neighbor->requests[irequest]->newton = 2;
  neighbor->requests[irequest]->ghost = 0;
}

/* ---------------------------------------------------------------------- */

void FixMBX::setup_post_neighbor()
{
  // setup after first neighbor build completes

  printf("\n[MBX] Inside setup_post_neighbor()\n");

  const int nlocal = atom->nlocal;
  const int nghost = atom->nghost;
  const int nall = nlocal + nghost;

  memory->create(mol_type,   nall, "fixmbx:mol_type");
  memory->create(mol_anchor, nall, "fixmbx:mol_anchor");
  
  tagint * tag = atom->tag;
  int * molecule = atom->molecule;
  double ** x = atom->x;

  // do we need to pre-compute and track molecule types?

  for(int i=0; i<nall; ++i) {
    const int id = tag[i];
    for(int j=0; j<num_mol_types; ++j)
      if(id <= mol_offset[j+1]) {
	mol_type[i] = j;
	break;
      }
  }

  // do we need to pre-compute and track anchor-atoms?

  for(int i=0; i<nall; ++i) {
    const int mol_id = molecule[i];
    const int mtype = mol_type[i];

    if( (tag[i] - 1 - mol_offset[mtype]) % num_atoms_per_mol[mtype] == 0) mol_anchor[i] = 1;
    else mol_anchor[i] = 0;
  }
  
  printf("per-atom info accessible\n");
  for(int i=0; i<nlocal; ++i) {
    printf("i= %i  tag= %lu  molecule= %i  mol_type= %i  mol_anchor= %i  x= %f %f %f\n",
	   i,tag[i],molecule[i],mol_type[i],mol_anchor[i],x[i][0],x[i][1],x[i][2]);
  }

  // create initial instance of MBX objects
  
  ptr_mbx = new bblock::System();
  ptr_mbx_full = new bblock::System();

  // loop over all atoms looking for anchor-atom of a molecule
  // -- assumption is that first atom in molecule is the anchor-atom
  // -- if anchor-atom is local, then molecule is treated as if local

  mbx_init();
  mbx_init_full();
}

/* ---------------------------------------------------------------------- */

void FixMBX::post_neighbor()
{
  // setup after neighbor build
  
  printf("\n[MBX] Inside post_neighbor()\n");
}

/* ---------------------------------------------------------------------- */

void FixMBX::setup_pre_force(int vflag)
{

  pre_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixMBX::min_setup_pre_force(int vflag)
{
  setup_pre_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixMBX::init_storage()
{

}

/* ---------------------------------------------------------------------- */

void FixMBX::pre_force(int /*vflag*/)
{

}

/* ---------------------------------------------------------------------- */

void FixMBX::min_pre_force(int vflag)
{
  pre_force(vflag);
}

/* ---------------------------------------------------------------------- */

int FixMBX::pack_forward_comm(int n, int *list, double *buf,
                                  int /*pbc_flag*/, int * /*pbc*/)
{
  int m;

  // if (pack_flag == 1)
  //   for(m = 0; m < n; m++) buf[m] = d[list[m]];
  // else if (pack_flag == 2)
  //   for(m = 0; m < n; m++) buf[m] = s[list[m]];
  // else if (pack_flag == 3)
  //   for(m = 0; m < n; m++) buf[m] = t[list[m]];
  // else if (pack_flag == 4)
  //   for(m = 0; m < n; m++) buf[m] = atom->q[list[m]];
  // else if (pack_flag == 5) {
  //   m = 0;
  //   for(int i = 0; i < n; i++) {
  //     int j = 2 * list[i];
  //     buf[m++] = d[j  ];
  //     buf[m++] = d[j+1];
  //   }
  //   return m;
  // }
  return n;
}

/* ---------------------------------------------------------------------- */

void FixMBX::unpack_forward_comm(int n, int first, double *buf)
{
  int i, m;

  // if (pack_flag == 1)
  //   for(m = 0, i = first; m < n; m++, i++) d[i] = buf[m];
  // else if (pack_flag == 2)
  //   for(m = 0, i = first; m < n; m++, i++) s[i] = buf[m];
  // else if (pack_flag == 3)
  //   for(m = 0, i = first; m < n; m++, i++) t[i] = buf[m];
  // else if (pack_flag == 4)
  //   for(m = 0, i = first; m < n; m++, i++) atom->q[i] = buf[m];
  // else if (pack_flag == 5) {
  //   int last = first + n;
  //   m = 0;
  //   for(i = first; i < last; i++) {
  //     int j = 2 * i;
  //     d[j  ] = buf[m++];
  //     d[j+1] = buf[m++];
  //   }
  // }
}

/* ---------------------------------------------------------------------- */

int FixMBX::pack_reverse_comm(int n, int first, double *buf)
{
  int i, m;
  //  if (pack_flag == 5) {
    // m = 0;
    // int last = first + n;
    // for(i = first; i < last; i++) {
    //   int indxI = 2 * i;
    //   buf[m++] = q[indxI  ];
    //   buf[m++] = q[indxI+1];
    // }
    // return m;
  //  } else {
    //    for (m = 0, i = first; m < n; m++, i++) buf[m] = q[i];
    return n;
    //  }
}

/* ---------------------------------------------------------------------- */

void FixMBX::unpack_reverse_comm(int n, int *list, double *buf)
{
  // if (pack_flag == 5) {
  //   int m = 0;
  //   for(int i = 0; i < n; i++) {
  //     int indxI = 2 * list[i];
  //     q[indxI  ] += buf[m++];
  //     q[indxI+1] += buf[m++];
  //   }
  // } else {
  //   for (int m = 0; m < n; m++) q[list[m]] += buf[m];
  // }
}

/* ----------------------------------------------------------------------
   memory usage of local atom-based arrays
------------------------------------------------------------------------- */

double FixMBX::memory_usage()
{
  double bytes;

  bytes = 0;

  return bytes;
}

/* ----------------------------------------------------------------------
   allocate fictitious charge arrays
------------------------------------------------------------------------- */

void FixMBX::grow_arrays(int nmax)
{
  memory->grow(mol_type,   nmax, "fixmbx:mol_type");
  memory->grow(mol_anchor, nmax, "fixmbx:mol_type");
}

/* ----------------------------------------------------------------------
   copy values within fictitious charge arrays
------------------------------------------------------------------------- */

void FixMBX::copy_arrays(int i, int j, int /*delflag*/)
{
  mol_type[j]   = mol_type[i];
  mol_anchor[j] = mol_anchor[i];
}

/* ----------------------------------------------------------------------
   pack values in local atom-based array for exchange with another proc
------------------------------------------------------------------------- */

int FixMBX::pack_exchange(int i, double *buf)
{
  int n = 0;
  buf[n++] = mol_type[i];
  buf[n++] = mol_anchor[i];
  return n;
}

/* ----------------------------------------------------------------------
   unpack values in local atom-based array from exchange with another proc
------------------------------------------------------------------------- */

int FixMBX::unpack_exchange(int nlocal, double *buf)
{
  int n = 0;
  mol_type[nlocal]   = buf[n++];
  mol_anchor[nlocal] = buf[n++];
  return n;
}

/* ----------------------------------------------------------------------
   Initialize new MBX instance with all molecules
------------------------------------------------------------------------- */

void FixMBX::mbx_init()
{
  //  printf("Inside mbx_init()\n");

  const int nlocal = atom->nlocal;
  const int nall = nlocal + atom->nghost;
  tagint * tag = atom->tag;
  double ** x = atom->x;
  
  std::vector<size_t> molec;

  // loop over all atoms on proc (local + ghost)

  int nm = 0;
  
  for(int i=0; i<nall; ++i) {
  //  for(int i=0; i<nlocal; ++i) {
      
      //    printf("i= %i  mol_anchor= %i\n",i,mol_anchor[i]);
    
    // if anchor-atom, then add to MBX (currently assume molecule is intact)
    
    if(mol_anchor[i]) {

      std::vector<std::string> names;
      std::vector<double> xyz;

      const int mtype = mol_type[i];

      int is_local = (i<nlocal);
      
      if(strcmp("h2o",mol_names[mtype]) == 0) {

	// add water molecule

	//printf(" -- Adding H2O\n");
	
	tagint anchor = tag[i];
	names.push_back("O");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);
	//	printf(" -- %i(i): O %f %f %f\n",anchor,i,x[i][0],x[i][1],x[i][2]);

	int ii = atom->map(anchor+1);
	if(ii < 0) error->all(FLERR,"H2O molecule not intact");

	names.push_back("H");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);
	//	printf(" -- %i(i): H %f %f %f\n",anchor,i,x[ii][0],x[ii][1],x[ii][2]);

	ii = atom->map(anchor+2);
	if(ii < 0) error->all(FLERR,"H2O molecule not intact");
	
	names.push_back("H");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);
	//	printf(" -- %i(i): H %f %f %f\n",anchor,i,x[ii][0],x[ii][1],x[ii][2]);

	molec.push_back(nm);
	nm++;

	ptr_mbx->AddMonomer(xyz, names, "h2o", is_local);
	ptr_mbx->AddMolecule(molec);

      } else if(strcmp("na",mol_names[mtype]) == 0) {
	
	// add sodium ion

	tagint anchor = tag[i];
	names.push_back("Na");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	molec.push_back(nm++);

	ptr_mbx->AddMonomer(xyz, names, "na", is_local);
	ptr_mbx->AddMolecule(molec);

      } else if(strcmp("cl",mol_names[mtype]) == 0) {
	
	// add chloride ion

	tagint anchor = tag[i];
	names.push_back("Na");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	molec.push_back(nm++);

	ptr_mbx->AddMonomer(xyz, names, "cl", is_local);
	ptr_mbx->AddMolecule(molec);

      } else if(strcmp("co2",mol_names[mtype]) == 0) {

	// add CO2 molecule

	tagint anchor = tag[i];
	names.push_back("C");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	int ii = atom->map(anchor+1);
	if(ii < 0) error->all(FLERR,"CO2 molecule not intact");

	names.push_back("O");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);

	ii = atom->map(anchor+2);
	if(ii < 0) error->all(FLERR,"CO2 molecule not intact");
	
	names.push_back("O");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);

	molec.push_back(nm++);

	ptr_mbx->AddMonomer(xyz, names, "co2", is_local);
	ptr_mbx->AddMolecule(molec);
	
      } else error->all(FLERR,"Unsupported molecule type in MBX"); // should never get this far...
	
    } // if(mol_anchor)
    
  } // for(i<nall)
  
  ptr_mbx->Initialize();
  
  // setup MBX solver(s); need to keep pbc turned off, which currently disables electrostatic solver

  std::vector<double> box;

  // if(!domain->nonperiodic) {
    
  //   box = std::vector<double>(9,0.0);
    
  //   box[0] = domain->xprd;

  //   box[3] = domain->xy;
  //   box[4] = domain->yprd;

  //   box[6] = domain->xz;
  //   box[7] = domain->yz;
  //   box[8] = domain->zprd;
    
  // } else if(domain->xperiodic || domain->yperiodic || domain->zperiodic)
  //   error->all(FLERR,"System must be fully periodic or non-periodic with MBX");
  
  ptr_mbx->SetPBC(box);

  // set MBX solvers
  
  ptr_mbx->SetDipoleMethod("cg");
  if (box.size()) {
    ptr_mbx->Set2bCutoff(9.0);
    ptr_mbx->SetEwaldElectrostatics(0.6, 2.5, 6);
    ptr_mbx->SetEwaldDispersion(0.5, 2.5, 6);
  } else {
    ptr_mbx->Set2bCutoff(100.0);
  }

}

/* ----------------------------------------------------------------------
   Initialize new MBX instance with local monomers as full system 
   (for debugging purposes)
------------------------------------------------------------------------- */

void FixMBX::mbx_init_full()
{
  //  printf("Inside mbx_init()\n");

  const int nlocal = atom->nlocal;
  const int nall = nlocal + atom->nghost;
  tagint * tag = atom->tag;
  double ** x = atom->x;
  
  std::vector<size_t> molec;

  // loop over all atoms on proc (local + ghost)

  int nm = 0;
  
  for(int i=0; i<nlocal; ++i) {
      
      //    printf("i= %i  mol_anchor= %i\n",i,mol_anchor[i]);
    
    // if anchor-atom, then add to MBX (currently assume molecule is intact)
    
    if(mol_anchor[i]) {

      std::vector<std::string> names;
      std::vector<double> xyz;

      const int mtype = mol_type[i];

      int is_local = (i<nlocal);
      
      if(strcmp("h2o",mol_names[mtype]) == 0) {

	// add water molecule

	//printf(" -- Adding H2O\n");
	
	tagint anchor = tag[i];
	names.push_back("O");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);
	//	printf(" -- %i(i): O %f %f %f\n",anchor,i,x[i][0],x[i][1],x[i][2]);

	int ii = atom->map(anchor+1);
	if(ii < 0) error->all(FLERR,"H2O molecule not intact");

	names.push_back("H");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);
	//	printf(" -- %i(i): H %f %f %f\n",anchor,i,x[ii][0],x[ii][1],x[ii][2]);

	ii = atom->map(anchor+2);
	if(ii < 0) error->all(FLERR,"H2O molecule not intact");
	
	names.push_back("H");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);
	//	printf(" -- %i(i): H %f %f %f\n",anchor,i,x[ii][0],x[ii][1],x[ii][2]);

	molec.push_back(nm);
	nm++;

	ptr_mbx_full->AddMonomer(xyz, names, "h2o", is_local);
	ptr_mbx_full->AddMolecule(molec);

      } else if(strcmp("na",mol_names[mtype]) == 0) {
	
	// add sodium ion

	tagint anchor = tag[i];
	names.push_back("Na");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	molec.push_back(nm++);

	ptr_mbx_full->AddMonomer(xyz, names, "na", is_local);
	ptr_mbx_full->AddMolecule(molec);

      } else if(strcmp("cl",mol_names[mtype]) == 0) {
	
	// add chloride ion

	tagint anchor = tag[i];
	names.push_back("Na");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	molec.push_back(nm++);

	ptr_mbx_full->AddMonomer(xyz, names, "cl", is_local);
	ptr_mbx_full->AddMolecule(molec);

      } else if(strcmp("co2",mol_names[mtype]) == 0) {

	// add CO2 molecule

	tagint anchor = tag[i];
	names.push_back("C");
	xyz.push_back(x[i][0]);
	xyz.push_back(x[i][1]);
	xyz.push_back(x[i][2]);

	int ii = atom->map(anchor+1);
	if(ii < 0) error->all(FLERR,"CO2 molecule not intact");

	names.push_back("O");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);

	ii = atom->map(anchor+2);
	if(ii < 0) error->all(FLERR,"CO2 molecule not intact");
	
	names.push_back("O");
	xyz.push_back(x[ii][0]);
	xyz.push_back(x[ii][1]);
	xyz.push_back(x[ii][2]);

	molec.push_back(nm++);

	ptr_mbx_full->AddMonomer(xyz, names, "co2", is_local);
	ptr_mbx_full->AddMolecule(molec);
	
      } else error->all(FLERR,"Unsupported molecule type in MBX"); // should never get this far...
	
    } // if(mol_anchor)
    
  } // for(i<nall)
  
  ptr_mbx_full->Initialize();
  
  // setup MBX solver(s)

  std::vector<double> box;

  if(!domain->nonperiodic) {
    
    box = std::vector<double>(9,0.0);
    
    box[0] = domain->xprd;
    
    box[3] = domain->xy;
    box[4] = domain->yprd;
    
    box[6] = domain->xz;
    box[7] = domain->yz;
    box[8] = domain->zprd;
    
  } else if(domain->xperiodic || domain->yperiodic || domain->zperiodic)
    error->all(FLERR,"System must be fully periodic or non-periodic with MBX");
  
  ptr_mbx_full->SetPBC(box);

  // set MBX solvers
  
  ptr_mbx_full->SetDipoleMethod("cg");
  if (box.size()) {
    ptr_mbx_full->Set2bCutoff(9.0);
    ptr_mbx_full->SetEwaldElectrostatics(0.6, 2.5, 6);
    ptr_mbx_full->SetEwaldDispersion(0.5, 2.5, 6);
  } else {
    ptr_mbx_full->Set2bCutoff(100.0);
  }

}

/* ----------------------------------------------------------------------
   Update old MBX instance for all molecules
------------------------------------------------------------------------- */

void FixMBX::mbx_update()
{
  /*
  double ** x = atom->x;
  
  std::vector<double> xyz(nmolecule*3*3);

  //  printf("nmolecule= %i\n",nmolecule);
  for(int i=0; i<nmolecule; ++i) {

    const int ia = i * 3 + 1; // hard-coded for water molecule
    const int im = i * 3 * 3;
    //    printf("molecule %i\n",i);
    
    int ii = atom->map(ia);
    //    printf("O %f %f %f\n",x[ii][0],x[ii][1],x[ii][2]);
    
    //    names.push_back("O");
    xyz[im  ] = x[ii][0];
    xyz[im+1] = x[ii][1];
    xyz[im+2] = x[ii][2];
    
    ii = atom->map(ia+1);
    //    printf("H %f %f %f\n",x[ii][0],x[ii][1],x[ii][2]);
    
    //    names.push_back("H");
    xyz[im+3] = x[ii][0];
    xyz[im+4] = x[ii][1];
    xyz[im+5] = x[ii][2];
    
    ii = atom->map(ia+2);
    //    printf("H %f %f %f\n",x[ii][0],x[ii][1],x[ii][2]);

    //    names.push_back("H");
    xyz[im+6] = x[ii][0];
    xyz[im+7] = x[ii][1];
    xyz[im+8] = x[ii][2];
  }

  // for(int i=0; i<nmolecule; ++i) {
  //   for(int j=0; j<3; ++j) {
  //     int indx = i*3+j;
  //     printf("%i  xyz= %f %f %f\n",indx,xyz[indx],xyz[indx+1],xyz[indx+2]);
  //   }
  // }
  
  ptr_mbx->SetRealXyz(xyz);
  */
}
