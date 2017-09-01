#!/bin/bash

if [ -z $CU_HOME ]; then
  echo "Please set the CU_HOME environment variable"
  exit
fi

echo "Running tests..."
cd tests
chmod 775 *.sh

for i in `ls test_*.sh`; do 
  bash $i
done

cd ../
