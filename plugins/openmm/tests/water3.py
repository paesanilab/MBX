#from simtk.openmm import System
#from mbnrgplugin import MBnrgForce
#system = System()
#force = MBnrgForce()
#system.addForce(force)

from simtk.openmm import app, System
import simtk.openmm as mm
from simtk import unit
import sys
from mbnrgplugin import MBnrgForce

pdb = app.PDBFile("water3.pdb")

forcefield = app.ForceField("/home/mrierari/codes/miniconda3/pkgs/openmm-7.2.0-py36_3/lib/python3.6/site-packages/simtk/openmm/app/data/mbnrg.xml")

print(pdb.topology)

system = forcefield.createSystem(pdb.topology)
force = MBnrgForce()
system.addForce(force)

integrator = mm.VerletIntegrator(0.00001*unit.femtoseconds)
platform = mm.Platform.getPlatformByName('Reference')
simulation = app.Simulation(pdb.topology, system, integrator, platform)
simulation.context.setPositions(pdb.positions)
