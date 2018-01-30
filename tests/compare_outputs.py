import numpy as np
import sys

def isFloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False

def separateFloats(l):
  lf = []
  ls = []
  for i in l:
    if isFloat(i):
      lf.append(float(i))
    else:
      ls.append(i)

  return lf,ls

if len(sys.argv) != 3:
  print("Usage: " + sys.argv[0] + " <file1> <file2>")
  sys.exit(0)

f1 = open(sys.argv[1],'r')
f2 = open(sys.argv[2],'r')

l1 = f1.readline()
l2 = f2.readline()

nl = 1

exit_code = 0
while l1 != '':
  a1, b1 = separateFloats(l1.split())
  a2, b2 = separateFloats(l2.split())
  if not np.allclose(a1,a2) or b1 != b2:
    print(sys.argv[1] + " and " + sys.argv[2] + " differ in line " + str(nl))
    print("<<<<< " + sys.argv[1])
    print(l1)
    print(">>>>> " + sys.argv[2])
    print(l2)
    exit_code = 1

  l1 = f1.readline()
  l2 = f2.readline()

  nl += 1

sys.exit(exit_code)
