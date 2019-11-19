#!/bin/bash

neq=`ls -d eq* | wc -w`

if [ $neq -lt 1 ]; then
  echo "No equilibration run!"
  exit
fi

nextp=$(($neq + 1))

cd eq$neq
squeue -u mrierari > qq.out
id=`cat jobid.dat`
#id=${id%.*}

if grep -q $id qq.out ; then
  echo "Job id $id is running or in queue"
  cd ../
  exit
fi

# Check if equilibration is done:
if [ ! -f "RESTART" ]; then
  echo "No RESTART FILE in eq$neq ... Maybe something went wrong"
  exit
fi

step=`grep "<step>" RESTART | sed 's/<step>//g' | sed 's/<\/step>//g' | awk '{print $1}'`
total_steps=`grep "<total_steps>" RESTART | sed 's/<total_steps>//g' | sed 's/<\/total_steps>//g' | awk '{print $1}'`

if [ $step -eq $total_steps ]; then
  echo "Equilibration is completed at eq$neq"
  mkdir ../prod1
  cat RESTART | sed "s/<step>$step<\/step>/<step>0<\/step>/g" | sed "s/<total_steps>$step<\/total_steps>/<total_steps>10000000<\/total_steps>/g"  > ../prod1/input.xml
  cp input.nrg ../prod1
  cp run.job ../prod1
  cd ../prod1
  sbatch run.job > remove_tmp 
  cat remove_tmp | awk '{print $4}' > jobid.dat
  rm remove_tmp
  echo "Submitted prod1"

else

  mkdir ../eq$nextp
  cp RESTART ../eq$nextp/input.xml
  cp input.nrg ../eq$nextp
  cp run.job ../eq$nextp
  cd ../eq$nextp
  qsub run.job > jobid.dat
  echo "Submitted eq$nextp (time: $step / $total_steps)"

fi
