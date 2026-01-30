# The MBX pair style for LAMMPS
MBX is interfaced with LAMMPS via pair_style MBX. For more information about MBX+LAMMPS, please refer to the [LAMMPS+MBX documentation](https://docs.lammps.org/pair_mbx.html).

Please see `plugins/lammps/examples` for example LAMMPS input files using MBX.

## pair_style format
To invoque the MBX pair_style, one must use the `pair_style mbx cutoff` keyword. The pair_style arguments are as follows:

* cutoff = real-space cutoff for MBX in Angstroms


Additionally, the `pair_coeff` command must be used to specify the monomer types and their corresponding atom mappings, as well as the MBX json configuration file. The format of the `pair_coeff` command is as follows:
```
pair_coeff * * num_mon_types mon_name atom_mapping <mon_name2> <atom_mapping2> ... json mbx.json print/settings
```

* num_mon_types = number of monomer types in the system
* mon_name = name of the monomer type (e.g. h2o, ch4, etc)
* atom mapping = list of LAMMPS atom IDs that correspond to the atoms in the monomer
* *json* arg = specifies the name of the MBX json configuration file, such as mbx.json
* print/settings = optionally print MBX settings to logfile

## CH4 and H2O example
For example, for a simulation of water and methane, where methane has types 1 and 2, and water has types 3 and 4 (C:1, Hc:2, O:3, Ho:4), the fix would look like this:
```
# For a system involving ch4 (atom types C=1, H=2) and
# water (atom types O=3, H=4)
pair_style      mbx 9.0
pair_coeff      * * 2 ch4 1 2 2 2 2 h2o 3 4 4 json mbx.json
compute         mbx all pair mbx
```

For more information about MBX+LAMMPS, please refer to the [LAMMPS+MBX documentation](https://docs.lammps.org/pair_mbx.html).