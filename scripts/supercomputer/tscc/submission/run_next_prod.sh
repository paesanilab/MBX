#!/bin/bash

nprod=`ls -d prod* | wc -w`

if [ $nprod -lt 1 ]; then
  echo "No production folder!"
  exit
fi

nextp=$(($nprod + 1))

cd prod$nprod
qstat -u $USER > qq.out
id=`cat jobid.dat`
id=${id%.*}

if grep -q $id qq.out ; then
  echo "Job id $id in $PWD is running or in queue"
  cd ../
  exit
fi

# Check if equilibration is done:
if [ ! -f "simulation.checkpoint" ]; then
  echo "No simulation.checkpoint in eq$nprod ... Maybe something went wrong"
  exit
fi

step=`grep "<step>" simulation.checkpoint | sed 's/<step>//g' | sed 's/<\/step>//g' | awk '{print $1}'`
total_steps=`grep "<total_steps>" simulation.checkpoint | sed 's/<total_steps>//g' | sed 's/<\/total_steps>//g' | awk '{print $1}'`

if [ $step -eq ${total_steps} ]; then
  echo "Simulation has been completed at $PWD"
else

  mkdir ../prod$nextp
  cp simulation.checkpoint ../prod$nextp/config.xml
  cp config.nrg ../prod$nextp
  cp run.job ../prod$nextp
  cp mbx.json ../prod$nextp
  cd ../prod$nextp
  qsub run.job > jobid.dat
  echo "Submitted prod$nextp for job in $PWD"
fi
