# Install/unInstall package files in LAMMPS
# mode = 0/1/2 for uninstall/install/update

mode=$1


# enforce using portable C locale
LC_ALL=C
export LC_ALL

# $1 = file, $2 = file it depends on
action () {
  file=$1
  dependency=$2
  
  if (test $mode = 0) then # uninstall
    rm -f ../$file
    echo "  uninstalling src/$file"

  elif (test -n "$dependency" && test ! -e ../$dependency) then
    echo "  ERROR:  src/$file missing dependency src/$dependency"
    exit 1
  
  elif (! cmp -s $file ../$file) then # file is different or missing
      if (test $mode = 1) then
        echo "  installing src/$file"
      else 
        echo "  updating src/$file"
      fi
      cp $file ../
  else
    echo "  src/$file is unchanged"
  fi

}

# list of files and necessary *.h dependencies

action pair_mbx.h
action pair_mbx.cpp pair_mbx.h

action fix_mbx.h
action fix_mbx.cpp fix_mbx.h

if (test mode != 0) then
  cp Makefile.mpi_mbx ../MAKE/
fi
