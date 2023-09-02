# Install/unInstall package files in LAMMPS
# mode = 0/1/2 for uninstall/update

mode=$1


# enforce using portable C locale
LC_ALL=C
export LC_ALL

# $1 = file, $2 = file it depends on
action () {
  if (test $mode = 0) then # uninstall
    rm -f ../$1
  elif (! cmp -s $1 ../$1) then
    if (test -z "$2" || test -e ../$2) then # only copy if dependency exists
      cp $1 ..
      if (test $mode = 1) then
        echo "  installing src/$1"
      else 
        echo "  updating src/$1"
      fi
    fi
  elif (test -n "$2") then # remove if missing dependency
    if (test ! -e ../$2) then
      echo "  removing src/$1 due to missing dependency src/$2"
      rm -f ../$1
    fi
  fi
}

# list of files with optional dependencies

action pair_mbx.h
action pair_mbx.cpp pair_mbx.h
action fix_mbx.h
action fix_mbx.cpp fix_mbx.h
