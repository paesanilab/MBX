#from simtk.openmm import System
#from mbnrgplugin import MBnrgForce
#system = System()
#force = MBnrgForce()
#system.addForce(force)

from simtk.openmm import app, System
import simtk.openmm as mm
from simtk import unit
import sys
import mbnrg


pdb = app.PDBFile("water3.pdb")
print(pdb.topology)

forcefield = app.ForceField(mbnrg.__file__.replace('mbnrg.py','mbnrg.xml'))

system = forcefield.createSystem(pdb.topology)
force = MBnrgForce()
system.addForce(force)

integrator = mm.VerletIntegrator(0.00001*unit.femtoseconds)
platform = mm.Platform.getPlatformByName('Reference')
simulation = app.Simulation(pdb.topology, system, integrator, platform)
simulation.context.setPositions(pdb.positions)
