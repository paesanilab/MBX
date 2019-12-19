WORK IN PROGRESS



The LAMMPS-MBX interface is organized as a LAMMPS package. The
USER-MBX directory should be copied into the lammps-<version>/src
directory.

*** To compile LAMMPS ***

The LAMMPS Makefile for your machine should be updated to point to the
MBX headers and libraries. An exmaple of a modified Makefile.mpi is
provided.

MBX = /path_to_mbx/

MBX_INC = -I$(MBX)/src -I$(MBX)/external
MBX_LIB = $(MBX)/../build_machine/install/lib/libmbxlib.a


CCFLAGS += (MBX_INC)
LIB += $(MBX_LIB)


To compile, simply install the USER-MBX package and use the updated
machine Makefile.

cd lammps-<version>/src
make yes-USER-MBX
make mpi_mbx -j 8


*** To run MBX simulation ***

Refer to provided examples for how to set the mbx fix and pair styles.

