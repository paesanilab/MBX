if ISMPI
TESTS = 
check_PROGRAMS = 
else
TESTS = unittest-angle unittest-bond unittest-buckingham unittest-bucktools unittest-connectivity unittest-dihedral unittest-dispersion unittest-disptools unittest-electrostatics unittest-energy1b unittest-energy2b unittest-energy3b unittest-energy4b unittest-energyff unittest-ffcalculators unittest-gamma unittest-inversion unittest-lj unittest-ljtools unittest-pme-nopolarization unittest-pme-nopolarization-findif unittest-pme-withpolarization unittest-pme-withpolarization-findif unittest-poly-1b unittest-poly-2b unittest-poly-3b unittest-poly-holder-1b unittest-poly-holder-2b unittest-poly-holder-3b unittest-ps unittest-system unittest-systools unittest-tools unittest-topology

check_PROGRAMS = unittest-angle unittest-bond unittest-buckingham unittest-bucktools unittest-connectivity unittest-dihedral unittest-dispersion unittest-disptools unittest-electrostatics unittest-energy1b unittest-energy2b unittest-energy3b unittest-energy4b unittest-energyff unittest-ffcalculators unittest-gamma unittest-inversion unittest-lj unittest-ljtools unittest-pme-nopolarization unittest-pme-nopolarization-findif unittest-pme-withpolarization unittest-pme-withpolarization-findif unittest-poly-1b unittest-poly-2b unittest-poly-3b unittest-poly-holder-1b unittest-poly-holder-2b unittest-poly-holder-3b unittest-ps unittest-system unittest-systools unittest-tools unittest-topology

LDADD = ${top_builddir}/src/libmbx.la 
AM_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) -I${top_builddir}/src/ $(EXTRA_CXXFLAGS_AM) -static

unittest_angle_SOURCES = unittest-main.cpp unittest-angle.cpp
unittest_bond_SOURCES = unittest-main.cpp unittest-bond.cpp
unittest_buckingham_SOURCES = unittest-main.cpp unittest-buckingham.cpp
unittest_bucktools_SOURCES = unittest-main.cpp unittest-bucktools.cpp
unittest_connectivity_SOURCES = unittest-main.cpp unittest-connectivity.cpp
unittest_dihedral_SOURCES = unittest-main.cpp unittest-dihedral.cpp
unittest_dispersion_SOURCES = unittest-main.cpp unittest-dispersion.cpp
unittest_disptools_SOURCES = unittest-main.cpp unittest-disptools.cpp
unittest_electrostatics_SOURCES = unittest-main.cpp unittest-electrostatics.cpp
unittest_energy1b_SOURCES = unittest-main.cpp unittest-energy1b.cpp
unittest_energy2b_SOURCES = unittest-main.cpp unittest-energy2b.cpp
unittest_energy3b_SOURCES = unittest-main.cpp unittest-energy3b.cpp
unittest_energy4b_SOURCES = unittest-main.cpp unittest-energy4b.cpp
unittest_energyff_SOURCES = unittest-main.cpp unittest-energyff.cpp
unittest_ffcalculators_SOURCES = unittest-main.cpp unittest-ffcalculators.cpp
unittest_gamma_SOURCES = unittest-main.cpp unittest-gamma.cpp
unittest_inversion_SOURCES = unittest-main.cpp unittest-inversion.cpp
unittest_lj_SOURCES = unittest-main.cpp unittest-lj.cpp
unittest_ljtools_SOURCES = unittest-main.cpp unittest-ljtools.cpp
unittest_pme_nopolarization_SOURCES = unittest-main.cpp unittest-pme-nopolarization.cpp
unittest_pme_nopolarization_findif_SOURCES = unittest-main.cpp unittest-pme-nopolarization-findif.cpp
unittest_pme_withpolarization_SOURCES = unittest-main.cpp unittest-pme-withpolarization.cpp
unittest_pme_withpolarization_findif_SOURCES = unittest-main.cpp unittest-pme-withpolarization-findif.cpp
unittest_poly_1b_SOURCES = unittest-main.cpp unittest-poly-1b.cpp
unittest_poly_2b_SOURCES = unittest-main.cpp unittest-poly-2b.cpp
unittest_poly_3b_SOURCES = unittest-main.cpp unittest-poly-3b.cpp
unittest_poly_holder_1b_SOURCES = unittest-main.cpp unittest-poly-holder-1b.cpp
unittest_poly_holder_2b_SOURCES = unittest-main.cpp unittest-poly-holder-2b.cpp
unittest_poly_holder_3b_SOURCES = unittest-main.cpp unittest-poly-holder-3b.cpp
unittest_ps_SOURCES = unittest-main.cpp unittest-ps.cpp
unittest_system_SOURCES = unittest-main.cpp unittest-system.cpp
unittest_systools_SOURCES = unittest-main.cpp unittest-systools.cpp
unittest_tools_SOURCES = unittest-main.cpp unittest-tools.cpp
unittest_topology_SOURCES = unittest-main.cpp unittest-topology.cpp
endif

