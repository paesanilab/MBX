import os,sys

if len(sys.argv) != 3:
  print("Usage: {} <configs_from_hole_detector.xyz> <max_BE>".format(sys.argv[0]))
  sys.exit()

confs = sys.argv[1]
maxbe = float(sys.argv[2])

with open(confs,'r') as ff, open("screened_" + confs,'w') as fo:
  line = ff.readline()
  while line != "":
    this_frame = ""
    natline = line
    nat = int(line.strip().split()[0])
    commentline = ff.readline()
    commentlist = commentline.strip().split()

    coord_text = ""
    for i in range(nat):
      coord_text += ff.readline()

    this_frame = natline + commentline + coord_text

    binding_energy = float(commentlist[12])

    if binding_energy < maxbe:
      fo.write(this_frame)

    line = ff.readline()
