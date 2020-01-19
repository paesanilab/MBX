#!/bin/bash

neq=`ls -d eq* | wc -w`

if [ $neq -lt 1 ]; then
  echo "No equilibration folder has been found!"
  exit
fi

nextp=$(($neq + 1))

cd eq$neq
qstat -u $USER > qq.out
id=`cat jobid.dat`
id=${id%.*}

if grep -q $id qq.out ; then
  echo "Job id $id is running or in queue"
  cd ../
  exit
fi

# Check if equilibration is done:
if [ ! -f "simulation.checkpoint" ]; then
  echo "No simulation.checkpoint in eq$neq ... Maybe something went wrong"
  exit
fi

step=`grep "<step>" simulation.checkpoint | sed 's/<step>//g' | sed 's/<\/step>//g' | awk '{print $1}'`
total_steps=`grep "<total_steps>" simulation.checkpoint | sed 's/<total_steps>//g' | sed 's/<\/total_steps>//g' | awk '{print $1}'`

if [ $step -eq $total_steps ]; then
  echo "Equilibration is completed at eq$neq"
  mkdir ../prod1
  cat simulation.checkpoint | sed "s/<step>$step<\/step>/<step>0<\/step>/g" | sed "s/<total_steps>$step<\/total_steps>/<total_steps>10000000<\/total_steps>/g"  > ../prod1/config.xml
  cp config.nrg ../prod1
  cp run.job ../prod1
  cp mbx.json ../prod1
  cd ../prod1
  qsub run.job > jobid.dat 
  echo "Submitted prod1"

else

  mkdir ../eq$nextp
  cp simulation.checkpoint ../eq$nextp/config.xml
  cp config.nrg ../eq$nextp
  cp run.job ../eq$nextp
  cp mbx.json ../eq$nextp
  cd ../eq$nextp
  qsub run.job > jobid.dat
  echo "Submitted eq$nextp (time: $step / $total_steps)"

fi
