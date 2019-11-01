import sys

if len(sys.argv) < 2:
    print("Usage: " + sys.argv[0] + "<input.nrg> [<unit_conversion = 1>]")
    sys.exit()

name = sys.argv[1].split(".nrg")[0]
if len(sys.argv) == 3:
    unit_conversion = float(sys.argv[2])
else:
    unit_conversion = 1.0

fin = open(sys.argv[1],'r')
nat = 0

ats = []
xyz = []
line = fin.readline()
while line != "":
    line2 = fin.readline()
    is_not_a_coordinate = line.strip().startswith("SYS") or line.strip().startswith("MON") or line.strip().startswith("MOL") or line.strip().startswith("END")
    if not is_not_a_coordinate:
        this_line = line.strip().split()
        ats.append(this_line[0])
        xyz.append([float(k) for k in this_line[1:]])
    if line2 == "":
        break
    line = line2
    
fin.close()

nats = len(ats)
fou = open(name + ".xyz",'w')
fou.write(str(nats) + " \n")
fou.write("0.0\n")
crd_list = "[ "
for i in range(nats):
    fou.write("{0:10s}{1:20.8f}{2:20.8f}{3:20.8f}\n".format(ats[i],xyz[i][0]*unit_conversion,xyz[i][1]*unit_conversion,xyz[i][2]*unit_conversion))
    crd_list += "{0:20.8f}, {1:20.8f}, {2:20.8f}".format(xyz[i][0]*unit_conversion,xyz[i][1]*unit_conversion,xyz[i][2]*unit_conversion)
    if i != nats - 1:
        crd_list += ", "
    else:
        crd_list += " ]\n"

print(crd_list)
fou.close()
