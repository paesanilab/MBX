SUBDIRS = . 
if !ISMPI
SUBDIRS +=  unittests
endif

# Libraries
lib_LTLIBRARIES = libmbx.la

# Library sources
# Sources from tools
tools_src = tools/math_tools.cpp tools/mpi_helper.cpp tools/mt19937.cpp tools/pbctools.cpp tools/properties.cpp tools/random-rotation.cpp tools/variable.cpp tools/water_monomer_lp.cpp tools/xmin.cpp

# Sources from io_tools
iotools_src = io_tools/read_connectivity.cpp io_tools/read_nrg.cpp io_tools/write_connectivity.cpp io_tools/read_xyz.cpp io_tools/write_xyz.cpp io_tools/write_nrg.cpp

# Sources from Building Block
bblock_src = bblock/external_call.cpp bblock/system.cpp bblock/sys_tools.cpp

# Make N2O5 its ouwn minilib to allow compilation with no opt
noinst_LTLIBRARIES = liba1b2c4.la
liba1b2c4_la_SOURCES = potential/1b/poly_1b_A1B2C4_deg5_v1.cpp potential/1b/poly_1b_A1B2C4_deg5_v1x.cpp potential/1b/x1b_A1B2C4_deg5_v1x.cpp potential/3b/poly_3b_A1B2C4_D1E2_D1E2_deg3_grad_v1.cpp potential/3b/poly_3b_A1B2C4_D1E2_D1E2_deg3_nograd_v1.cpp 
liba1b2c4_la_CXXFLAGS = $(EXTRA_CXXFLAGS_AM) -O0

# 1B poly sources
oneb_sources = potential/1b/energy1b.cpp potential/1b/mbnrg_1b_A1B3_deg5_v1.cpp potential/1b/mbnrg_1b_A2_deg8_v1.cpp potential/1b/poly_1b_A1B2_deg4_v1x.cpp potential/1b/poly_1b_A1B3_deg5_grad_v1.cpp potential/1b/poly_1b_A1B3_deg5_nograd_v1.cpp potential/1b/poly_1b_A1B4_deg5_exp0_v1.cpp potential/1b/poly_1b_A1B4_deg5_exp0_v1x.cpp potential/1b/poly_1b_A2_deg8_grad_v1.cpp potential/1b/poly_1b_A2_deg8_nograd_v1.cpp potential/1b/ps.cpp potential/1b/x1b_A1B2_deg4_v1x.cpp potential/1b/x1b_A1B4_deg5_exp0_v1x.cpp potential/1b/poly_1b_A1B2_deg6_grad_vmbpbe.cpp potential/1b/poly_1b_A1B2_deg6_nograd_vmbpbe.cpp  potential/1b/mbnrg_1b_A1B2_deg6_vmbpbe.cpp

# 2B poly sources
twob_sources = potential/2b/energy2b.cpp potential/2b/mbnrg_2b_A1_A1_deg9_v1.cpp potential/2b/mbnrg_2b_A1_B1C2X2_deg5_v1.cpp potential/2b/mbnrg_2b_A1_B1_deg9_v1.cpp potential/2b/mbnrg_2b_A1_B2_deg7_v1.cpp potential/2b/mbnrg_2b_A1B2Z2_C2_deg5_v1.cpp potential/2b/mbnrg_2b_A1B3_A1B3_deg4_v1.cpp potential/2b/mbnrg_2b_A2_A2_deg6_v1.cpp potential/2b/poly_2b_A1_A1_deg9_grad_v1.cpp potential/2b/poly_2b_A1_A1_deg9_nograd_v1.cpp potential/2b/poly_2b_A1_B1C2X2_deg5_grad_v1.cpp potential/2b/poly_2b_A1_B1C2X2_deg5_nograd_v1.cpp potential/2b/poly_2b_A1_B1_deg9_grad_v1.cpp potential/2b/poly_2b_A1_B1_deg9_nograd_v1.cpp potential/2b/poly_2b_A1B2_A1B2_v1.cpp potential/2b/poly_2b_A1B2_A1B2_v1x.cpp potential/2b/poly_2b_A1B2C4_D1E2_deg3_v1.cpp potential/2b/poly_2b_A1B2C4_D1E2_deg3_v1x.cpp potential/2b/poly_2b_A1_B2_deg7_grad_v1.cpp potential/2b/poly_2b_A1_B2_deg7_nograd_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D2_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D2_v1x.cpp potential/2b/poly_2b_A1B2Z2_C1D4_deg3_exp0_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp potential/2b/poly_2b_A1B2Z2_C2_deg5_grad_v1.cpp potential/2b/poly_2b_A1B2Z2_C2_deg5_nograd_v1.cpp potential/2b/poly-2b-A1B2Z2_C-v1.cpp potential/2b/poly-2b-A1B2Z2_C-v1x.cpp potential/2b/poly_2b_A1B3_A1B3_deg4_grad_v1.cpp potential/2b/poly_2b_A1B3_A1B3_deg4_nograd_v1.cpp potential/2b/poly_2b_A1B4_A1B4_deg4_exp0_v1.cpp potential/2b/poly_2b_A1B4_A1B4_deg4_exp0_v1x.cpp potential/2b/poly_2b_A2_A2_deg6_grad_v1.cpp potential/2b/poly_2b_A2_A2_deg6_nograd_v1.cpp potential/2b/poly-2b-v6x.cpp potential/2b/poly-2b-v6x-nogrd.cpp potential/2b/x2b_A1B2_A1B2_v1x.cpp potential/2b/x2b_A1B2C4_D1E2_deg3_v1x.cpp potential/2b/x2b_A1B2Z2_C1D2_v1x.cpp potential/2b/x2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp potential/2b/x2b-A1B2Z2_C-v1x.cpp potential/2b/x2b_A1B4_A1B4_deg4_exp0_v1x.cpp potential/2b/x2b-v9x.cpp potential/2b/mbnrg_2b_A1B2Z2_A1B2Z2_deg4_vmbpbe.cpp potential/2b/poly_2b_A1B2Z2_A1B2Z2_deg4_nograd_vmbpbe.cpp potential/2b/poly_2b_A1B2Z2_A1B2Z2_deg4_grad_vmbpbe.cpp potential/2b/mbnrg_2b_A1B4_C1D2_deg4_v1.cpp potential/2b/poly_2b_A1B4_C1D2_deg4_nograd_v1.cpp potential/2b/poly_2b_A1B4_C1D2_deg4_grad_v1.cpp potential/2b/x2b_A1_A1_deg23_v1x.cpp potential/2b/poly_2b_A1_A1_deg23_v1.cpp potential/2b/poly_2b_A1_A1_deg23_v1x.cpp potential/2b/x2b_A1B2Z2_C1_deg5_v1x.cpp potential/2b/poly_2b_A1B2Z2_C1_deg5_v1.cpp potential/2b/poly_2b_A1B2Z2_C1_deg5_v1x.cpp

# 3B poly sources
threeb_sources = potential/3b/energy3b.cpp potential/3b/mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v1.cpp potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_v1.cpp potential/3b/mbnrg_3b_A1B2C4_D1E2_D1E2_deg3_v1.cpp potential/3b/mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_grad_v1.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_nograd_v1.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_grad_v1.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_nograd_v1.cpp potential/3b/poly_3b_A1B4_C1D2_C1D2_deg3_grad_v1.cpp potential/3b/poly_3b_A1B4_C1D2_C1D2_deg3_nograd_v1.cpp potential/3b/poly-3b-h2o-ion-v1x_deg4_filtered.cpp potential/3b/poly-3b-v2x.cpp potential/3b/poly-3b-v2x-nogrd.cpp potential/3b/x3b-h2o-ion-v1x-deg4-filtered.cpp potential/3b/x3b-v2x.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_grad_vmbpbe.cpp potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_vmbpbe.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_nograd_vmbpbe.cpp potential/3b/mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v1_oldswitch.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_grad_v1_oldswitch.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_nograd_v1_oldswitch.cpp

# 4B poly sources
fourb_sources = potential/4b/energy4b.cpp  potential/4b/mbnrg_4b_A1B2_A1B2_A1B2_A1B2_deg4_v1.cpp  potential/4b/poly_4b_A1B2_A1B2_A1B2_A1B2_deg4_grad_v1.cpp  potential/4b/poly_4b_A1B2_A1B2_A1B2_A1B2_deg4_nograd_v1.cpp

# Electrostatics sources
elec_sources = potential/electrostatics/electrostatics.cpp potential/electrostatics/fields.cpp potential/electrostatics/gammq.cpp

# Dispersion sources
disp_sources = potential/dispersion/dispersion.cpp potential/dispersion/disptools.cpp

# Buckingham sources
buck_sources = potential/buckingham/buckingham.cpp potential/buckingham/bucktools.cpp

# FF sources
ff_sources = potential/force_field/angles.cpp potential/force_field/bond.cpp potential/force_field/calculators.cpp potential/force_field/connectivity.cpp potential/force_field/dihedral.cpp potential/force_field/energyff.cpp potential/force_field/inversion.cpp potential/force_field/topology.cpp

# LJ sources
lj_sources = potential/lj/lj.cpp potential/lj/ljtools.cpp

# Potential sources
potential_src = $(oneb_sources) $(twob_sources) $(threeb_sources) $(fourb_sources) $(elec_sources) $(disp_sources) $(buck_sources) $(ff_sources) $(lj_sources)

libmbx_la_SOURCES = $(tools_src) $(iotools_src) $(bblock_src) $(potential_src)
libmbx_la_LIBADD = liba1b2c4.la 

# Library extra flags
libmbx_la_CPPFLAGS = $(CPPFLAGS) $(MPIFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM)
libmbx_la_LDFLAGS = $(LDFLAGS) $(LIBS) $(EXTRA_LDFLAGS_AM)

# Headers (the ‘nobase_’ prefix can be used to circumvent this path stripping)
nobase_include_HEADERS = tools/*.h io_tools/*.h bblock/*.h potential/1b/*.h potential/2b/*.h potential/3b/*.h potential/4b/*.h potential/buckingham/*.h potential/dispersion/*.h potential/electrostatics/*.h potential/force_field/*.h potential/lj/*.h json/json.h kdtree/*.h* 


# Only compile these binaries if is not MPI
if !ISMPI
# Main Binaries
bin_PROGRAMS = single_point optimize order_frames mb_decomp

if HASGSL
bin_PROGRAMS += normal_modes
endif

# binary sources
single_point_SOURCES = main/single_point.cpp
optimize_SOURCES = main/optimize.cpp
order_frames_SOURCES = main/order_frames.cpp
mb_decomp_SOURCES = main/mb_decomp.cpp

# Binary Include path
single_point_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM) -I$(top_srcdir)/src -static
optimize_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM) -I$(top_srcdir)/src -static
order_frames_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM) -I$(top_srcdir)/src -static
mb_decomp_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM) -I$(top_srcdir)/src -static

# Binary dependencies
single_point_DEPENDENCIES = libmbx.la
single_point_LDADD = libmbx.la

optimize_DEPENDENCIES = libmbx.la
optimize_LDADD = libmbx.la

order_frames_DEPENDENCIES = libmbx.la
order_frames_LDADD = libmbx.la

mb_decomp_DEPENDENCIES = libmbx.la
mb_decomp_LDADD = libmbx.la

if HASGSL
normal_modes_SOURCES = main/normal-modes.cpp
normal_modes_CXXFLAGS = $(CPPFLAGS) $(EXTRA_CXXFLAGS_AM) $(OPENMP_CXXFLAGS) -I$(top_srcdir)/src -static
normal_modes_DEPENDENCIES = libmbx.la
normal_modes_LDADD = libmbx.la
endif
endif



