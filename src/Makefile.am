# Libraries
lib_LTLIBRARIES = libmbx.la

# Library sources
# Sources from tools
tools_src = tools/math_tools.cpp tools/mpi_helper.cpp tools/mt19937.cpp tools/pbctools.cpp tools/properties.cpp tools/random-rotation.cpp tools/variable.cpp tools/water_monomer_lp.cpp tools/xmin.cpp

# Sources from io_tools
iotools_src = io_tools/read_connectivity.cpp io_tools/read_nrg.cpp io_tools/write_connectivity.cpp io_tools/read_xyz.cpp io_tools/write_xyz.cpp io_tools/write_nrg.cpp

# Sources from Building Block
bblock_src = bblock/external_call.cpp bblock/system.cpp bblock/sys_tools.cpp

# 1B poly sources
oneb_sources = potential/1b/energy1b.cpp potential/1b/mbnrg_1b_A1B3_deg5_v1.cpp potential/1b/mbnrg_1b_A2_deg8_v1.cpp potential/1b/poly_1b_A1B2C4_deg5_v1.cpp potential/1b/poly_1b_A1B2C4_deg5_v1x.cpp potential/1b/poly_1b_A1B2_deg4_v1x.cpp potential/1b/poly_1b_A1B3_deg5_grad_v1.cpp potential/1b/poly_1b_A1B3_deg5_nograd_v1.cpp potential/1b/poly_1b_A1B4_deg5_exp0_v1.cpp potential/1b/poly_1b_A1B4_deg5_exp0_v1x.cpp potential/1b/poly_1b_A2_deg8_grad_v1.cpp potential/1b/poly_1b_A2_deg8_nograd_v1.cpp potential/1b/ps.cpp potential/1b/x1b_A1B2C4_deg5_v1x.cpp potential/1b/x1b_A1B2_deg4_v1x.cpp potential/1b/x1b_A1B4_deg5_exp0_v1x.cpp

# 2B poly sources
twob_sources = potential/2b/energy2b.cpp potential/2b/mbnrg_2b_A1_A1_deg9_v1.cpp potential/2b/mbnrg_2b_A1_B1C2X2_deg5_v1.cpp potential/2b/mbnrg_2b_A1_B1_deg9_v1.cpp potential/2b/mbnrg_2b_A1_B2_deg7_v1.cpp potential/2b/mbnrg_2b_A1B2Z2_C2_deg5_v1.cpp potential/2b/mbnrg_2b_A1B3_A1B3_deg4_v1.cpp potential/2b/mbnrg_2b_A2_A2_deg6_v1.cpp potential/2b/poly_2b_A1_A1_deg9_grad_v1.cpp potential/2b/poly_2b_A1_A1_deg9_nograd_v1.cpp potential/2b/poly_2b_A1_B1C2X2_deg5_grad_v1.cpp potential/2b/poly_2b_A1_B1C2X2_deg5_nograd_v1.cpp potential/2b/poly_2b_A1_B1_deg9_grad_v1.cpp potential/2b/poly_2b_A1_B1_deg9_nograd_v1.cpp potential/2b/poly_2b_A1B2_A1B2_v1.cpp potential/2b/poly_2b_A1B2_A1B2_v1x.cpp potential/2b/poly_2b_A1B2C4_D1E2_deg3_v1.cpp potential/2b/poly_2b_A1B2C4_D1E2_deg3_v1x.cpp potential/2b/poly_2b_A1_B2_deg7_grad_v1.cpp potential/2b/poly_2b_A1_B2_deg7_nograd_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D2_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D2_v1x.cpp potential/2b/poly_2b_A1B2Z2_C1D4_deg3_exp0_v1.cpp potential/2b/poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp potential/2b/poly_2b_A1B2Z2_C2_deg5_grad_v1.cpp potential/2b/poly_2b_A1B2Z2_C2_deg5_nograd_v1.cpp potential/2b/poly-2b-A1B2Z2_C-v1.cpp potential/2b/poly-2b-A1B2Z2_C-v1x.cpp potential/2b/poly_2b_A1B3_A1B3_deg4_grad_v1.cpp potential/2b/poly_2b_A1B3_A1B3_deg4_nograd_v1.cpp potential/2b/poly_2b_A1B4_A1B4_deg4_exp0_v1.cpp potential/2b/poly_2b_A1B4_A1B4_deg4_exp0_v1x.cpp potential/2b/poly_2b_A2_A2_deg6_grad_v1.cpp potential/2b/poly_2b_A2_A2_deg6_nograd_v1.cpp potential/2b/poly-2b-v6x.cpp potential/2b/poly-2b-v6x-nogrd.cpp potential/2b/x2b_A1B2_A1B2_v1x.cpp potential/2b/x2b_A1B2C4_D1E2_deg3_v1x.cpp potential/2b/x2b_A1B2Z2_C1D2_v1x.cpp potential/2b/x2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp potential/2b/x2b-A1B2Z2_C-v1x.cpp potential/2b/x2b_A1B4_A1B4_deg4_exp0_v1x.cpp potential/2b/x2b-v9x.cpp

# 3B poly sources
threeb_sources = potential/3b/energy3b.cpp potential/3b/mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v1.cpp potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_v1.cpp potential/3b/mbnrg_3b_A1B2C4_D1E2_D1E2_deg3_v1.cpp potential/3b/mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_grad_v1.cpp potential/3b/poly_3b_A1_B1C2X2_B1C2X2_deg4_nograd_v1.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_grad_v1.cpp potential/3b/poly_3b_A1B2_A1B2_A1B2_deg4_nograd_v1.cpp potential/3b/poly_3b_A1B2C4_D1E2_D1E2_deg3_grad_v1.cpp potential/3b/poly_3b_A1B2C4_D1E2_D1E2_deg3_nograd_v1.cpp potential/3b/poly_3b_A1B4_C1D2_C1D2_deg3_grad_v1.cpp potential/3b/poly_3b_A1B4_C1D2_C1D2_deg3_nograd_v1.cpp potential/3b/poly-3b-h2o-ion-v1x_deg4_filtered.cpp potential/3b/poly-3b-v2x.cpp potential/3b/poly-3b-v2x-nogrd.cpp potential/3b/x3b-h2o-ion-v2x-deg4-filtered.cpp potential/3b/x3b-v2x.cpp

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
potential_src = $(oneb_sources) $(twob_sources) $(threeb_sources) $(elec_sources) $(disp_sources) $(buck_sources) $(ff_sources) $(lj_sources)

libmbx_la_SOURCES = $(tools_src) $(iotools_src) $(bblock_src) $(potential_src)

# Library extra flags
libmbx_la_CPPFLAGS = $(FFTWFLAGS) 

# Headers (the ‘nobase_’ prefix can be used to circumvent this path stripping)
nobase_include_HEADERS = tools/*.h io_tools/*.h bblock/*.h potential/1b/*.h potential/2b/*.h potential/3b/*.h potential/buckingham/*.h potential/dispersion/*.h potential/electrostatics/*.h potential/force_field/*.h potential/lj/*.h 

include_HEADERS = $(top_buildidr)/external/json/json.h $(top_buildidr)/external/kdtree/*.h* $(top_buildidr)/external/Catch2/single_include/catch.hpp

## Binaries
#bin_PROGRAMS = main_exe
#
## Binary sources
#main_exe_SOURCES = main/main.cpp
#
## Binary Include path
#main_exe_CPPFLAGS = $(FFTWFLAGS)
#
## Binary dependencies
#main_exe_DEPENDENCIES = libmym.la
##main_exe_DEPENDENCIES = $(top_builddir)/.lib/libmym.a
#main_exe_LDADD = libmym.la



