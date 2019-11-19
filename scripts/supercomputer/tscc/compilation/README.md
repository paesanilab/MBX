# Before installation (Last Update 11/18/2019)
In order to install MBX on tscc, you will need to make sure you have the following:
- CMake 3.12.4 or higher. The CMake version installed on TSCC is CMake 3.9.1. You can load extra "unsuported" modules by using `export MODULEPATH=/projects/builder-group/jpg/modulefiles/applications:$MODULEPATH` , followed by `module load cmake/3.13.4`. This is in the `compile_tscc.sh` script.
- Intel compilers v2017 or higher. Currently, the intel compilers on TSCC are v2016, and there will be compilation errors when using those. For now, there is no way on getting intel v2017, so we will have to compile with GNU compilers.

# Installation
You can try to install MBX at the login node. For that,
`cp $MBX_HOME/scripts/supercomputers/tscc/compile_tscc.sh $MBX_HOME`
`cd $MBX_HOME`
`./compile_tscc.sh gnu`

If there are problems to compile or takes too long, you can submit the compilation as a job.
First, set `MBX_HOME` to the right path in `scripts/supercomputers/tscc/compile_tscc_gnu.job`.
Then,
`cp $MBX_HOME/scripts/supercomputers/tscc/compile_tscc_gnu.job $MBX_HOME`
`cd $MBX_HOME`
`qsub compile_tscc_gnu.job`

# After installation
Now remember to compile the driver.
`cd $MBX_HOME/plugins/i-pi/src/main`
`cp Makefile_tscc Makefile`
`make`
