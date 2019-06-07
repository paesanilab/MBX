#!/bin/bash

directories="150co2_150h2o  h2o_in_277co2  nh4pf6_in_277h2o  co2_in_277h2o  nh4_in_277h2o  pf6_in_277h2o  "
#directories="nh4_in_277h2o pf6_in_277h2o"

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
