#!/usr/bin/python3

import os
import math
import sys 

if len(sys.argv) != 2: sys.exit("Usage: python gaussian_spectrum.py lmon.dat > gaussian_spectrum.dat")  

lmonfilename=sys.argv[1]
with open(lmonfilename) as lmonfile :
    lineslmon=lmonfile.read().splitlines()

gauss_spectrum=[0.0]*4000
sigma=2

for freq in [float(i) for i in range(4000)]:
  for j in range(len(lineslmon)) : 
    exponent= math.pow((float(lineslmon[j].split()[0])-float(freq)),2)/(2*math.pow(sigma,2))
    gauss= float(lineslmon[j].split()[1])*math.exp(0.0-exponent);
    gauss_spectrum[int(freq)]+=gauss

for freq in range(4000) :
	print(freq, gauss_spectrum[freq])
