import sys

if len(sys.argv) < 4:
    print("Usage: " + sys.argv[0] + "<lmptraj> <id1> <at1> <id2> <at2> ...")
    sys.exit()

name = sys.argv[1]
id_col = 2
xyz_col = 4

id_eq = {}
for i in range(2,len(sys.argv),2):
    id_eq[int(sys.argv[i])] = sys.argv[i+1]

fin = open(sys.argv[1],'r')
fou = open("traj.xyz",'w')

line = fin.readline()

ats = []
while True:
    if line == "":
        break
    fin.readline()
    fin.readline()
    line = fin.readline()
    nat = int(line.strip().split()[0])
    fin.readline()
    xbl = fin.readline().strip().split()
    xbox = float(xbl[1]) - float(xbl[0])
    ybl = fin.readline().strip().split()
    ybox = float(ybl[1]) - float(ybl[0])
    zbl = fin.readline().strip().split()
    zbox = float(zbl[1]) - float(zbl[0])
    fin.readline()
    
    xyz = []
    ids = []
    for i in range(nat):
        line = fin.readline().strip().split()
        ids.append(line[id_col])
        xyz.append([float(k) for k in line[xyz_col:xyz_col+3]])

    if len(ats) == 0:
        for i in ids:
            ats.append(id_eq[int(i)])

    line = fin.readline()

    fou.write("{}\n{} {} {}\n".format(nat,xbox,ybox,zbox))

    for i in range(nat):
        fou.write("{} {} {} {}\n".format(ats[i],xyz[i][0],xyz[i][1],xyz[i][2]))

fin.close()
fou.close()
