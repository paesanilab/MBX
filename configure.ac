#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Require a version of Autoconf more recent than 2.69
AC_PREREQ([2.69])

# Initialize AC
AC_INIT([MBX], [1.1.3], [mbx-users@googlegroups.com])

# Put the auxiliary files in a different folder to not overcrowd the main directory
AC_CONFIG_AUX_DIR([build-aux])

# Directory where the M4 macros are. Created automatically.
AC_CONFIG_MACRO_DIR([m4])

# Look for the archiver AR to make the static lib
AM_PROG_AR

# Initialize the LIBTOOLS lib to make the .la libraries, from where the .so and .a will be created
LT_INIT([disable-shared])

# Initialize Automake. Warnings will be treated as errors.
AM_INIT_AUTOMAKE([subdir-objects -Wall foreign])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/unittests/Makefile
 plugins/Makefile
 plugins/i-pi/Makefile
])

# Helper function to add the proper flags to CPPFLAGS and LDFLAGS
# MBX_CHECK_PACKAGE(path_to_home)
AC_DEFUN([MBX_CHECK_PACKAGE], [
    # Check if a path to the home dir of lib has been given
    if ! test $1 == none; then
        LDFLAGS="$LDFLAGS -L$1/lib"
        CPPFLAGS="$CPPFLAGS -I$1/include"
    fi
])

# Helper function to see if compiler accepts a flag
# MBX_CHECK_CXXFLAG(flag)
# use it to check if a flag is available on this compiler
AC_DEFUN([MBX_CHECK_CXXFLAG], [
  save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $1"
  AC_MSG_CHECKING([whether $CXX accepts $1])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [
      AC_LINK_IFELSE([AC_LANG_PROGRAM([])],
        [AC_MSG_RESULT([yes]); EXTRA_CXXFLAGS="$EXTRA_CXXFLAGS $1"],
        [AC_MSG_RESULT([not linking]); CXXFLAGS="$save_CXXFLAGS"])
    ],
    [AC_MSG_RESULT([no]); CXXFLAGS="$save_CXXFLAGS"]
  )
  CXXFLAGS="$save_CXXFLAGS"
])

# MBX_CHECK_REQ_CXXFLAG(flag)
# use it to check if a required flag is available on this compiler
AC_DEFUN([MBX_CHECK_REQ_CXXFLAG], [
  save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $1"
  AC_MSG_CHECKING([whether $CXX accepts $1])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [
      AC_LINK_IFELSE([AC_LANG_PROGRAM([])],
        [AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([not linking]); AC_MSG_ERROR([please ensure that your $CXX compiler is able to accept $1])])
    ],
    [AC_MSG_RESULT([no]); AC_MSG_ERROR([please ensure that your $CXX compiler is able to accept $1])]
  )
  CXXFLAGS="$save_CXXFLAGS"
  EXTRA_CXXFLAGS="$EXTRA_CXXFLAGS $1"
])

# Helper function to set the enables
# MBX_CONFIG_ENABLE(variablename,optionname,doc,default)
# notice that variablename and optionname are likely identical,
# they just need to be different with optionname contains a "-"
# (not allowed in shell variable names)
AC_DEFUN([MBX_CONFIG_ENABLE], [
m4_bpatsubst([$1],-,_)=
AC_ARG_ENABLE([$1],
  AS_HELP_STRING([--enable-$1], [enable $2, default: $3]),
  [case "${enableval}" in
             (yes) m4_bpatsubst([$1],-,_)=true ;;
             (no)  m4_bpatsubst([$1],-,_)=false ;;
             (*)   AC_MSG_ERROR([wrong argument to --enable-$1]) ;;
  esac],
  [case "$3" in
             (yes) m4_bpatsubst([$1],-,_)=true ;;
             (no)  m4_bpatsubst([$1],-,_)=false ;;
  esac]
)
])

# Everything is in C++, so are the tests
AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
CXXFLAGS=''
EXTRA_CXXFLAGS=''
AC_OPENMP

# Check compiler is working
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [],
    [AC_MSG_ERROR([C++ compiler ${CXX} is not working])]
)

# Checks for libraries.

AC_CHECK_LIB([m],[cos])
AC_SEARCH_LIBS([omp_get_thread_num],[iomp5 gomp])


# FFTW
# Look for possible environment variables known to have FFTW home
fftw_dir=none
fftw_dir_name=none
if test "$FFTW_HOME" != ""; then
    fftw_dir="$FFTW_HOME"
    fftw_dir_name="FFTW_HOME"
elif test "$FFTWHOME" != ""; then 
    fftw_dir="$FFTWHOME"
    fftw_dir_name="FFTWHOME"
elif test "$FFTW_ROOT" != "" ; then
    fftw_dir="$FFTW_ROOT"
    fftw_dir_name="FFTW_ROOT"
elif test "$FFTWROOT" != "" ; then
    fftw_dir="$FFTWROOT"
    fftw_dir_name="FFTWROOT"
fi
AC_MSG_NOTICE([fftw_dir: ${fftw_dir_name}=${fftw_dir}])
MBX_CHECK_PACKAGE([$fftw_dir])

AC_CHECK_LIB([fftw3],[fftw_plan_dft],[
    CPPFLAGS="$CPPFLAGS -DHAVE_FFTWD=1"
    LIBS="-lfftw3 $LIBS"
],[
    AC_MSG_ERROR([fftw3 cannot be linked. Please define FFTW_HOME.])
])

# GSL
# Look for possible environment variables known to have GSL home
gsl_dir=none
if test "$GSL_HOME" != ""; then
    gsl_dir="$GSL_HOME"
elif test "$GSLHOME" != ""; then
    gsl_dir="$GSLHOME"
elif test "$GSL_ROOT" != "" ; then
    gsl_dir="$GSL_ROOT"
elif test "$GSLROOT" != "" ; then
    gsl_dir="$GSLROOT"
elif test "$GSL_ROOT_DIR" != "" ; then
    gsl_dir="$GSL_ROOT_DIR"
fi

MBX_CHECK_PACKAGE([$gsl_dir])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm],[
    LIBS="-lgsl $LIBS"
    AM_CONDITIONAL([HASGSL], [test x == x])
],[
    AM_CONDITIONAL([HASGSL], [test x == y])
    AC_MSG_WARN([gsl cannot be found. Will not install the normal modes executable.])
])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
# Check for C++11
#MBX_CHECK_REQ_CXXFLAG([-std=c++17])

# Check for enable flags
MBX_CONFIG_ENABLE([debug],[debugging],[no])
MBX_CONFIG_ENABLE([verbose],[verbose debug output],[no])
MBX_CONFIG_ENABLE([optimization],[compile with optimization],[yes])
MBX_CONFIG_ENABLE([mpi],[compile with mpi],[no])
MBX_CONFIG_ENABLE([coverage],[compile with coverage flags],[no])
MBX_CONFIG_ENABLE([optreport],[write optimization reports],[no])
MBX_CONFIG_ENABLE([ttmnrg],[enable ttm-nrg calculations],[no])
MBX_CONFIG_ENABLE([cpp17],[enable C++17 standard, allowing some additional optimizations],[no])
MBX_CONFIG_ENABLE([avx2],[enable the avx2 vector instruction set, improving vetorization],[no])
MBX_CONFIG_ENABLE([avx512],[enable the avx-512 vector instruction set, further improving vetorization],[no])

# Act on enable flags

if test $debug == true; then
    MBX_CHECK_REQ_CXXFLAG([-g]) # allow debugging using gdb
fi

if test $verbose == true; then
    MBX_CHECK_CXXFLAG([-DDEBUG]) # verbose debug output
fi

if test $optimization == true; then
    MBX_CHECK_REQ_CXXFLAG([-O2])
else
    MBX_CHECK_REQ_CXXFLAG([-O0])
fi

if test $coverage == true; then
    MBX_CHECK_CXXFLAG([--coverage]) # allow coverage analysis using codecov
fi

# deprecated ttm-nrg mode
if test $ttmnrg == true; then
    AC_MSG_WARN([ttm-nrg is deprecated.])
    MBX_CHECK_CXXFLAG([-DTTMNRG])
fi

if test $optreport == true; then
    MBX_CHECK_CXXFLAG([-qopt-report=1])
    MBX_CHECK_CXXFLAG([-qopt-report-phase=vec])
fi

if test $cpp17 == true; then
    MBX_CHECK_REQ_CXXFLAG([-std=c++17])
else
    MBX_CHECK_REQ_CXXFLAG([-std=c++11])
fi

if test $avx2 == true; then
    MBX_CHECK_REQ_CXXFLAG([-xCORE-AVX2])
fi

if test $avx512 == true; then
    MBX_CHECK_REQ_CXXFLAG([-xCORE-AVX512])
fi

# Add flags to variable
AC_SUBST(EXTRA_CXXFLAGS_AM,["$EXTRA_CXXFLAGS "])

# MPI
# Set AutoMake flag to true
AM_CONDITIONAL([ISMPI], [test $mpi == true])
if test $mpi == true; then
    # Look for possible environment variables known to have MPI home
    mpi_dir=none
    mpi_dir_name=none
    if test "$MPI_HOME" != ""; then
        mpi_dir="$MPI_HOME"
        mpi_dir_name="MPI_HOME"
    elif test "$MPIHOME" != ""; then
        mpi_dir="$MPIHOME"
        mpi_dir_name="MPIHOME"
    elif test "$MPI_ROOT" != "" ; then
        mpi_dir="$MPI_ROOT"
        mpi_dir_name="MPI_ROOT"
    elif test "$MPIROOT" != "" ; then
        mpi_dir="$MPIROOT"
        mpi_dir_name="MPIROOT"
    fi
    AC_MSG_NOTICE([mpi_dir: ${mpi_dir_name}=${mpi_dir}])
    MBX_CHECK_PACKAGE([$mpi_dir])
    AC_CHECK_LIB([mpi],[MPI_Init],[
        CPPFLAGS="$CPPFLAGS -DHAVE_MPI=1"
        LIBS="-lmpi $LIBS"
    ],
    [
        AC_MSG_ERROR([mpi lib is missing])
    ])
    AC_MSG_CHECKING([if compiler ${CXX} is able to find mpi.h])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <mpi.h>]])],[AC_MSG_RESULT(yes)], [
        AC_MSG_RESULT(no)
        AC_MSG_ERROR([compiler ${CXX} is not able to find mpi.h])
        ])
fi

# Checks for library functions.

AC_PREFIX_DEFAULT([`pwd`])

AC_OUTPUT
