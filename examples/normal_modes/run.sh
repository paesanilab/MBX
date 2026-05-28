#!/bin/bash

module load gcc

../../bin/normal_modes input.nrg mbx.json 1 > frequencies.dat

