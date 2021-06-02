from math import log, exp

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
