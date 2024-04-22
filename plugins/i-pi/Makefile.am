# Only compile these binaries if is not MPI
if !ISMPI
# Main Binaries
bin_PROGRAMS = ipi_driver


# binary sources
ipi_driver_SOURCES = src/main/driver.cpp

# Binary Include path
ipi_driver_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) $(EXTRA_CXXFLAGS_AM) -I$(top_srcdir)/src -static

# Binary dependencies
LDADD = ${top_builddir}/src/libmbx.la 
AM_CXXFLAGS = $(CPPFLAGS) $(OPENMP_CXXFLAGS) -I${top_builddir}/src/ $(EXTRA_CXXFLAGS_AM) -static

install-exec-hook:
	mkdir -p bin
	ln -sf ../ipi_driver bin/driver

endif



