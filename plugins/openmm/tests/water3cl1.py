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


pdb = app.PDBFile("water3cl1.pdb")
print(pdb.topology)

forcefield = app.ForceField(mbnrg.__file__.replace('mbnrg.py', 'mbnrg.xml'))

system = forcefield.createSystem(pdb.topology)

integrator = mm.VerletIntegrator(0.00001*unit.femtoseconds)
platform = mm.Platform.getPlatformByName('Reference')
simulation = app.Simulation(pdb.topology, system, integrator, platform)
simulation.context.setPositions(pdb.positions)

state = simulation.context.getState(getForces=True, getEnergy=True)
potential_energy = state.getPotentialEnergy()

print("Energy:")
print(potential_energy.in_units_of(unit.kilocalorie_per_mole))
print("Gradients")
kilocalorie_per_mole_per_angstrom = unit.kilocalorie_per_mole/unit.angstrom
for f in state.getForces():
    print(f.in_units_of(kilocalorie_per_mole_per_angstrom))

