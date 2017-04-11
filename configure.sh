#!/bin/bash

cd src/bblock
make clean
make
cd ../tools
make clean
make
cd ../main
make clean
make
