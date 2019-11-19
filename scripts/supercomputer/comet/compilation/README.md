# Before installation (Last Update 11/19/2019)
In order to install MBX on comet, you will need to make sure you have the following:
- CMake 3.12.4 or higher. The CMake version installed on COMET is CMake 3.9.1. Best way to get CMake is to download it and install it on your home 
- Intel compilers v2017 or higher. Currently, the intel compilers on COMET are v2016, and there will be compilation errors when using those. For now, there is no way on getting intel v2017, so we will have to compile with GNU compilers.

# Installation
You can try to install MBX at the login node. For that,
`cp $MBX_HOME/scripts/supercomputers/comet/compilation/compile_comet.sh $MBX_HOME`
`cd $MBX_HOME`
Then update the `CMAKE_EXE` variable to point to the right cmake executable and do:
`./compile_comet.sh gnu`

If there are problems to compile or takes too long, you can submit the compilation as a job.
First, set `MBX_HOME` to the right path and make `CMAKE_EXE` point to the right executable in `scripts/supercomputers/comet/compilation/compile_tscc_gnu.job`.
Then,
`cp $MBX_HOME/scripts/supercomputers/comet/compilation/compile_tscc_gnu.job $MBX_HOME`
`cd $MBX_HOME`
`sbatch compile_tscc_gnu.job`

# After installation
Now remember to compile the driver.
`cd $MBX_HOME/plugins/i-pi/src/main`
`cp Makefile_comet Makefile`
`make`
