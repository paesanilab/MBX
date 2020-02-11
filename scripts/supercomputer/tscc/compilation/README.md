# Before installation (Last Update 11/18/2019)
In order to install MBX on tscc, you will need to make sure you have the following:
- CMake 3.12.4 or higher. The CMake version installed on TSCC is CMake 3.9.1. You can load extra "unsuported" modules by using `export MODULEPATH=/projects/builder-group/jpg/modulefiles/applications:$MODULEPATH` , followed by `module load cmake/3.13.4`. This is in the `compile_tscc.sh` script.
- Intel compilers v2017 or higher. The default compilers are 2016, but one can load the 2018 compilers (see compilation scripts)

# Installation
You can try to install MBX at the login node. For that,
`cp $MBX_HOME/scripts/supercomputers/tscc/compilation/compile_tscc.sh $MBX_HOME`
`cd $MBX_HOME`
`./compile_tscc.sh intel`

If there are problems to compile or takes too long, you can submit the compilation as a job.
First, set `MBX_HOME` to the right path in `scripts/supercomputers/tscc/compile_tscc.job`.
Then,
`cp $MBX_HOME/scripts/supercomputers/tscc/compilation/compile_tscc.job $MBX_HOME`
`cd $MBX_HOME`
`qsub compile_tscc.job`

This will compile both the driver and the MBX library.
