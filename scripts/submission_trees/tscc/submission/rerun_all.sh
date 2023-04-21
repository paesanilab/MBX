#!/bin/bash

directories="system1 system2"

CURRDIR=$PWD

for folder in $directories ; do
  cd $CURRDIR
  cd $folder
  nruns=`ls -d run* | wc -w`
  for i in `seq 1 1 $nruns`; do
    cd $CURRDIR/$folder/run$i
    if [ ! -d "prod1" ]; then
      echo "$folder/run$i still equilibrating"
      $CURRDIR/run_next_eq.sh
    else
      echo "$folder/run$i still running"
      $CURRDIR/run_next_prod.sh
    fi
    echo "------------------------------------------------------------------"
  done
done
