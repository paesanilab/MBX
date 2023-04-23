# The MBX fix for LAMMPS
MBX is interfaced with LAMMPS via a pair/fix combination. The source files are in `USER-MBX`.

## Variables that can be printed
LAMMPS can access some energy contributions from MBX, and one can print them in the output. To enable the variable calculation, one must add to the LAMMPS input the following lines:
```
compute         mbx all pair mbx
variable        e1    equal c_mbx[1]
variable        e2    equal c_mbx[2]
variable        e3    equal c_mbx[3]
variable        e4    equal c_mbx[4]
variable        ebuck equal c_mbx[5]
variable        edisp equal c_mbx[6]
variable        eele  equal c_mbx[7]
variable        etot  equal c_mbx[8]
```
This will store the one- (e1), two- (e2), three- (e3), and foru-body (e4) energies, along with the deprecated classical repultion (ebuck), the dispersion energy (edisp), the electrostatic energu, both permanent and induced added together (eele), and the total energy.

One can define a thermo style such as:
```
thermo_style    custom step time temp cella cellb cellc evdwl ecoul epair ebond eangle edihed eimp emol elong etail v_e1 v_e2 v_e3 v_ebuck v_edisp v_eele v_etot pe etotal
```
to print each of the contributions.

## Fix format
To invoque the MBX fix, one must use the `fix ID mbx` keyword. The fix arguments are as follows:
`fix   ID  all mbx <number_of_monomer_types> <monomer_id_1> <lower_atom_type_index_of_mon1> <higher_atom_type_index_of_mon1> <number_of_atoms_of_mon1> <atom type of atom 1> <atom type of atom 2> ... <atom type of atom n1> <monomer_id_2> ... <monomer_id_N> ...  json <name_of_json_file>`

For example, for a simulation of water and methane, where methane has types 1 and 2, and water has types 3 and 4 (C:1, Hc:2, O:3, Ho:4), the fix would look like this:
`fix    1  all mbx 2 ch4 1 2 5 1 2 2 2 2 h2o 3 4 3 3 4 4 json mbx.json`
