#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Require a version of Autoconf more recent than 2.69
AC_PREREQ([2.69])

# Initialize AC
AC_INIT([MBX], [1.0], [mbx-users@googlegroups.com])

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
])

# Helper function to check if a required library can be linked
# MBX_CHECK_REQ_LIB(header,program,lib,path_to_home)
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
        AC_MSG_ERROR([please make sure that the library $3 is properly installed. Please define the environment variable FFTW_HOME and rerun ./configure])
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

    if test $linkinggood == no; then
        AC_MSG_ERROR([please make sure that the library $3 is properly installed. Please define the environment variable FFTW_HOME and rerun ./configure])
    fi
])
    
# Everything is in C++, so are the tests
AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

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

],[fftw3],[$fftw_dir])

# Add the corresponding flags to the variable FFTWFLAGS
if test $fftw_dir != none; then
    AC_SUBST(FFTWFLAGS,["-L$fftw_dir/lib -I$fftw_dir/include"])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_PREFIX_DEFAULT([`pwd`/install])

AC_OUTPUT
