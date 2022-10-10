#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Require a version of Autoconf more recent than 2.69
AC_PREREQ([2.69])

# Initialize AC
AC_INIT([MBX], [0.7], [mbx-users@googlegroups.com])

# Put the auxiliary files in a different folder to not overcrowd the main directory
AC_CONFIG_AUX_DIR([build-aux])

# Directory where the M4 macros are. Created automatically.
AC_CONFIG_MACRO_DIR([m4])

# Look for the archiver AR to make the static lib
AM_PROG_AR

# Initialize the LIBTOOLS lib to make the .la libraries, from where the .so and .a will be created
LT_INIT([disable-shared])

# Initialize Automake. Warnings will be treated as errors.
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/unittests/Makefile
])

# Helper function to check if a required library can be linked
# MBX_CHECK_REQ_LIB(header,program,lib,path_to_home,environmentVar)
AC_DEFUN([MBX_CHECK_REQ_PACKAGE], [
    # Check if a path to the home dir of lib has been given
    if ! test $4 == none; then
        header_file=$4/include/$1
        LDFLAGS="-I$4/include -L$4/lib $LDFLAGS"
    else
        header_file=$1
    fi
    AC_MSG_CHECKING([whether the header $1 can be found])
    headersgood=no
    AC_CHECK_HEADERS($header_file,[headersgood=yes])
    
    if test $headersgood == no; then
        AC_CHECK_FILE($1,[headersgood=yes])
        if test $headersgood == no; then
            AC_MSG_ERROR([please make sure that the library $3 is properly installed. Please define the environment variable $5 and rerun ./configure])
        fi
    fi

    bk_LIBS="$LIBS"
    LIBS="-l$3 $LIBS"
    
    linkinggood=no
    AC_MSG_CHECKING([whether MBX can be linked to $3])
    AC_LINK_IFELSE([AC_LANG_SOURCE([$2])],
          [linkinggood=yes]
          [AC_MSG_RESULT([yes])],
          [AC_MSG_RESULT([no])]
    )

    if test $linkinggood == no;then
        AC_MSG_ERROR([please make sure that the library $3 is properly installed. Please define the environment variable $5 and rerun ./configure])
    fi
])

# Helper function to check if gsl library can be linked
# MBX_CHECK_GSL_LIB(program,path_to_home,environmentVar)
AC_DEFUN([MBX_CHECK_GSL_PACKAGE], [
    # Check if a path to the home dir of lib has been given
    header_file=gsl/gsl_math.h
    if ! test $2 == none; then
	header_file=$2/include/gsl/gsl_math.h
        LDFLAGS="-I$2/include -L$2/lib $LDFLAGS"
    fi
    AC_MSG_CHECKING([whether the header gsl/gsl_math.h can be found])
    headersgood=no
    AC_CHECK_HEADERS($header_file,[headersgood=yes])

    if test $headersgood == no; then
        AC_CHECK_FILE([$header_file],[headersgood=yes])
        if test $headersgood == no; then
            AC_MSG_ERROR([please make sure that the library gsl is properly installed. Please define the environment variable $3 and rerun ./configure])
        fi
    fi

    bk_LIBS="$LIBS"
    LIBS="-lgslcblas -lgsl $LIBS"

    linkinggood=no
    AC_MSG_CHECKING([whether MBX can be linked to gsl and gslcblas])
    AC_LINK_IFELSE([AC_LANG_SOURCE([$1])],
          [linkinggood=yes]
          [AC_MSG_RESULT([yes])],
          [AC_MSG_RESULT([no])]
    )

    if test $linkinggood == no;then
        AC_MSG_ERROR([please make sure that the library gsl is properly installed. Please define the environment variable $3 and rerun ./configure])
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
# use it to check if a flag is available on this compiler
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

# Checks for libraries.

# FFTW
# Look for possible environment variables known to have FFTW home
fftw_dir=none
if test "$FFTW_HOME" != ""; then
    fftw_dir="$FFTW_HOME"
elif test "$FFTWHOME" != ""; then 
    fftw_dir="$FFTWHOME"
elif test "$FFTW_ROOT" != "" ; then
    fftw_dir="$FFTW_ROOT"
elif test "$FFTWROOT" != "" ; then
    fftw_dir="$FFTWROOT"
fi
MBX_CHECK_REQ_PACKAGE([fftw3.h],[
#include <fftw3.h>

int main() {
    fftw_complex nc;
    return 0;
}

],[fftw3],[$fftw_dir],["FFTW_HOME"])

# Add the corresponding flags to the variable FFTWFLAGS
if test $fftw_dir != none; then
    AC_SUBST(FFTWFLAGS,["-L$fftw_dir/lib -I$fftw_dir/include -DHAVE_FFTWD=1"])
else
    AC_SUBST(FFTWFLAGS,["-DHAVE_FFTWD=1"])
fi

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
MBX_CHECK_GSL_PACKAGE([
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

int main() {
    double a = M_SQRT3;
    gsl_vector av;
    return 0;
}

],[$gsl_dir],["GSL_HOME"])

# Add the corresponding flags to the variable FFTWFLAGS
if test $gsl_dir != none; then
    AC_SUBST(GSLFLAGS,["-L$gsl_dir/lib -I$gsl_dir/include"])
else
    AC_SUBST(GSLFLAGS,[""])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
# Check for C++11
MBX_CHECK_REQ_CXXFLAG([-std=c++11])

# Check for enable flags
MBX_CONFIG_ENABLE([debug],[debugging],[no])
MBX_CONFIG_ENABLE([verbose],[verbose debug output],[no])
MBX_CONFIG_ENABLE([optimization],[compile with optimization],[yes])
MBX_CONFIG_ENABLE([mpi],[compile with mpi],[no])
MBX_CONFIG_ENABLE([coverage],[compile with coverage flags],[no])
MBX_CONFIG_ENABLE([optreport],[write optimization reports].[no])

# Act on enable flags

if test $debug == true; then
    MBX_CHECK_CXXFLAG([-g])
fi

if test $verbose == true; then
    MBX_CHECK_CXXFLAG([-DDEBUG])
fi

if test $optimization == true; then
    MBX_CHECK_CXXFLAG([-O2])
else
    MBX_CHECK_CXXFLAG([-O0])
fi

if test $coverage == true; then
    MBX_CHECK_CXXFLAG([--coverage])
fi

if test $optreport == true; then
    MBX_CHECK_CXXFLAG([-qopt-report=1])
    MBX_CHECK_CXXFLAG([-qopt-report-phase=vec])
fi

# Add flags to variable
AC_SUBST(EXTRA_CXXFLAGS_AM,["$EXTRA_CXXFLAGS "])

# MPI
# Set AutoMake flag to true
AM_CONDITIONAL([ISMPI], [test x$mpi = xtrue])
if test $mpi == true; then
    # Look for possible environment variables known to have FFTW home
    mpi_dir=none
    if test "$MPI_HOME" != ""; then
        mpi_dir="$MPI_HOME"
    elif test "$MPIHOME" != ""; then
        mpi_dir="$MPIHOME"
    elif test "$MPI_ROOT" != "" ; then
        mpi_dir="$MPI_ROOT"
    elif test "$MPIROOT" != "" ; then
        mpi_dir="$MPIROOT"
    fi
    MBX_CHECK_REQ_PACKAGE([mpi.h],[
#include <mpi.h>

int main(int argc, char **argv){
  MPI_Init(&argc,&argv);
  return MPI_Finalize();
}
    ],[mpi],[$mpi_dir],["MPI_HOME"])
    
    # Add the corresponding flags to the variable FFTWFLAGS
    if test $mpi_dir != none; then
        AC_SUBST(MPIFLAGS,["-L$mpi_dir/lib -I$mpi_dir/include -DHAVE_MPI=1"])
    else
        AC_SUBST(MPIFLAGS,["-DHAVE_MPI=1"])
    fi
fi

# Checks for library functions.

AC_PREFIX_DEFAULT([`pwd`/install])

AC_OUTPUT
