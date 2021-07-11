import sys,os

from mbfit.polynomials import PolynomialGenerator

if len(sys.argv) != 2:
    print("Usage: python script.py <symmetry>")
    exit()

os.system("touch x.ini")

symmetry = sys.argv[1]
pg = PolynomialGenerator("x.ini")
atom_permutations = list(pg.get_permutations(symmetry))

os.system("rm x.ini")

with open("permutations.txt",'w') as ff:
    for perm in atom_permutations:
        for p in perm:
            ff.write("{} ".format(p))
        ff.write("\n")

