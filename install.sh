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
cd ../potential/1b
make clean
make
cd ../../../test/src-test
make clean
make
cd ../../
