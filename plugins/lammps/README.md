WORK IN PROGRESS

*** MBX library/headers ***

Build MBX for your system as you normally would.

An extra step that is currently needed/helpful, is to copy the
libmbxlib.a library to an easier location for linking. The LAMMPS
Makefile below assumes the MBX library was copied to the
build/install/lib directory. An example follows.

cd /home/bin/MBX/build
make -j 8
make install
mkdir install/lib
cp stage/home/bin/MBX/install/lib/static/libmbxlib.a ./install/lib

Something similar could be done for copying headers to an
./install/include directory as well.



*** LAMMPS ***

The LAMMPS-MBX interface is organized as a LAMMPS package. The
USER-MBX directory should be copied into the lammps-<version>/src
directory.

*** To compile LAMMPS ***

The LAMMPS Makefile for your machine should be updated to point to the
MBX headers and libraries. An exmaple of a modified Makefile.mpi is
provided.

MBX = /path_to_mbx/

CCFLAGS += -I$(MBX)/src -I$(MBX)/external
LIB += $(MBX)/build/install/lib/libmbxlib.a


To compile, install the USER-MBX package and use the updated machine
Makefile.

cd lammps-<version>/src
make yes-USER-MBX
make mpi_mbx -j 8


*** To run MBX simulation ***

Refer to provided examples for how to set the mbx fix and pair styles.

