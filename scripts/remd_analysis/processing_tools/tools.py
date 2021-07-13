from math import log, exp

def mass_to_atom(m):
  if m < 0 :
    print("Negative mass not possible") 
    sys.exit()
  elif m < 1.01 :
    name = "H";
  elif m < 2.02 :
    name = "D";
  elif m < 7.02 :
    name = "Li";
  elif m < 16.00 :
    name = "O";
  elif m < 19.00 :
    name = "F";
  elif m < 23.00 :
    name = "Na";
  elif m < 35.00 :
    name = "Cl";
  elif m < 39.0 :
    name = "K";
  elif m < 79.00 :
    name = "Br";
  elif m < 85.00 :
    name = "Rb";
  elif m < 127.00:
    name = "I";
  elif m < 133.00:
    name = "Cs";
  else:
    print("Mass not in database.")
    sys.exit()

  return name;

def read_xyz(filename):
  ats = []
  xyz = []
  
  with open(filename,'r') as ff:
    nat = int(ff.readline().strip().split()[0])
    ff.readline()
    for i in range(nat):
      ll = ff.readline().strip().split()
      ats.append(ll[0])
      xyz.append([float(k) for k in ll[1:]])

  return ats, xyz

def get_remd_temperatures(tmin,tmax,nrep):
  temps = []

  log_T_min = log(tmin)
  log_T_max = log(tmax)
  delta = (log_T_max - log_T_min) / (nrep - 1)
  
  log_T = log_T_min
  for i in range(nrep):
      temps.append(exp(log_T_min))
      log_T_min += delta

  return temps

def read_trajectory_frame_to_string(ff):
  temp = ""
  frame_string = ""
  end_code = ""

  line = ff.readline()
  if line == "":
    end_code = "done"
    return end_code, temp, frame_string
  if line.strip() != '{time}':
    end_code = "error"
    return end_code, temp, frame_string

  frame_string += line
  line = ff.readline()
  while line.strip() != '-------------------end-of-frame-------------------':
    if line.strip() == '{temperature}':
      frame_string += line
      line = ff.readline()
      temp = line.strip()
    frame_string += line
    line = ff.readline()

    if line == "":
      end_code = "error"
      return end_code, "", ""

  frame_string += line

  return end_code, temp, frame_string

def read_trajectory_frame(ff):
  time = ""
  temp = ""
  mass = []
  positions = []
  end_code = ""

  line = ff.readline()
  if line == "":
    end_code = "done"
    return end_code,positions,mass,temp,time
  if line.strip() != '{time}':
    print("notime", line)
    end_code = "error"
    return end_code,positions,mass,temp,time
  
  line = ff.readline()
  time = line.strip()

  while line.strip() != '-------------------end-of-frame-------------------':
    if "temperature" in line:
      line = ff.readline()
      temp = line.strip()
      line = ff.readline()
    elif "mass" in line:
      line = ff.readline()
      while not "{" in line:
        thismasses = [float(k) for k in line.strip().split()]
        for k in thismasses:
          mass.append(k)
        line = ff.readline()
    elif "positions" in line:
      line = ff.readline()
      while not "{" in line and not "end-of-frame" in line:
        thispos = [float(k) for k in line.strip().split()]
        for k in thispos:
          positions.append(k)
        line = ff.readline()
    elif line == "":
      print("Something went wrong")
      sys.exit()
    else:
      # Should not get here
      line = ff.readline()

  end_code = ""

  return end_code,positions,mass,temp,time

def find_centroid_from_pimd(xyz,nbeads):
  nats = len(xyz)/nbeads/3
  nats = int(nats)
  centroid_xyz = [0.0] * (3*nats)
  for i in range(nbeads):
    for j in range(3*nats):
      centroid_xyz[j] += xyz[3*nats*i + j]

  xyz_out = [i/nbeads for i in centroid_xyz]

  return xyz_out

def write_xyz(ff, nat, ats, xyz, comment = "0.0"):
  ff.write("{}\n".format(nat))
  ff.write("{}\n".format(comment))
  for i in range(nat):
    ff.write("{0:<8}{1:18.8E}{2:18.8E}{3:18.8E}\n".format(ats[i], xyz[3*i],xyz[3*i+1],xyz[3*i+2]))

