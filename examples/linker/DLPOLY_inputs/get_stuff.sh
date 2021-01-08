#!/bin/bash

for i in `grep -A 11 BONDS FIELD | tail -n 11 | awk '{print "{"$4"}"}'`; do a="${a},$i" ; done; echo "LINEAR PARAMS BONDS: $a "

a=""

for i in `grep -A 11 BONDS FIELD | tail -n 11 | awk '{print "{"$5"}"}'`; do a="${a},$i" ; done; echo "NON LINEAR PARAMS BONDS: $a "

a=""

for i in `grep -A 16 ANGLES FIELD | tail -n 16 | awk '{print "{"$5"}"}'`; do a="${a},$i" ; done; echo "LINEAR PARAMS ANGLES: $a "

a=""

for i in `grep -A 16 ANGLES FIELD | tail -n 16 | awk '{print "{"$6"}"}'`; do a="${a},$i" ; done; echo "NON LINEAR PARAMS ANGLES: $a "

a=""

for i in `grep -A 24 DIHEDRALS FIELD | tail -n 24 | awk '{print "{"$6"}"}'`; do a="${a},$i" ; done; echo "LINEAR PARAMS DIHEDRALS: $a "

a=""

for i in `grep -A 24 DIHEDRALS FIELD | tail -n 24 | awk '{print "{"$8","$7/180.0*3.14159265359"}"}'`; do a="${a},$i" ; done; echo "NON LINEAR PARAMS DIHEDRALS: $a "

a=""

for i in `grep -A 24 DIHEDRALS FIELD | tail -n 24 | awk '{print "{"$8",3.14159265359}"}'`; do a="${a},$i" ; done; echo "NON LINEAR PARAMS DIHEDRALS: $a "
