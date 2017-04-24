#!/bin/bash

source include/install/config.sh

cd src/bblock
make clean
make
cd ../tools
make clean
make
cd ../main
make clean
make
cd ../../test/src-test
make clean
make
cd ../../
