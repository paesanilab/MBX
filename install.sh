#!/bin/bash

source src/install/config.sh

cd src/tools
make clean
make
cd ../bblock
make clean
make
cd ../io_tools
make clean
make
#cd ../main
#make clean
#make
cd ../../test/src-test
make clean
make
cd ../../
