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

from simtk.openmm import LocalEnergyMinimizer

LocalEnergyMinimizer.minimize(simulation.context, 1e-1)

state = simulation.context.getState(getForces=True, getEnergy=True, getPositions=True)
potential_energy = state.getPotentialEnergy()
potential_energy.in_units_of(unit.kilocalorie_per_mole)

simulation.context.setVelocitiesToTemperature(300*unit.kelvin)
# Equilibrate
simulation.step(10)

simulation.reporters.append(app.StateDataReporter(sys.stdout, 10, step=True,
    potentialEnergy=True, temperature=True, progress=True, remainingTime=True,
    speed=True, totalSteps=110, separator='\t'))


# Add a `PDBReporter` that writes molecules positions every 20 steps in a pdb file.

# In[ ]:

simulation.reporters.append(app.PDBReporter('trajectory.pdb', 20))

simulation.step(100)
