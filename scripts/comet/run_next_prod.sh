#!/bin/bash

nprod=`ls -d prod* | wc -w`

if [ $nprod -lt 1 ]; then
  echo "No production run!"
  exit
fi

nextp=$(($nprod + 1))

cd prod$nprod
squeue -u mrierari > qq.out
id=`cat jobid.dat`
#id=${id%.*}

if grep -q $id qq.out ; then
  echo "Job id $id in $PWD is running or in queue"
  cd ../
  exit
fi

mkdir ../prod$nextp
cp RESTART ../prod$nextp/input.xml
cp input.nrg ../prod$nextp
cp run.job ../prod$nextp
cd ../prod$nextp
qsub run.job > jobid.dat
echo "Submitted prod$nextp for job in $PWD"
