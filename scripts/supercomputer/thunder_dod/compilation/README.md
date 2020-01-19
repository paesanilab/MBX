# Before installation (Last Update 01/16/2020)
In order to install MBX on thunder, you will need to make sure you have the following:
- CMake 3.12.4 or higher. The CMake version installed on TSCC is CMake 3.5.1. I Recommend to download the CMake source code and install it. The latest release from the official CMake website should work and do the job.
- Intel compilers v2017 or higher. Intel compilers on thunder are v2019, so we are fine here. 

# Installation
You can try to install MBX at the login node. For that,
`cp $MBX_HOME/scripts/supercomputers/tscc/compilation/compile_thunder_XX.sh $MBX_HOME`
`cd $MBX_HOME`
`./compile_thunder_XX.sh gnu`

Where XX can either be gnu or intel (intel is strongly recomended)

If there are problems to compile or takes too long, you can submit the compilation as a job.
First, set `MBX_HOME` to the right path in `scripts/supercomputers/tscc/compile_thunder_XX.job`.
Then,
`cp $MBX_HOME/scripts/supercomputers/tscc/compilation/compile_thunder_XX.job $MBX_HOME`
`cd $MBX_HOME`
`qsub compile_thunder_XX.job`

# After installation
Now remember to compile the driver. Set up the environment variable `MBX_HOME`:
NOTE: Thunder's default shell is csh!! export won't work.
`setenv MBX_HOME $HOME/codes/MBX`
If you want to use bash, you can type `bash` in the terminal and the shell is gonna change to bash. Then you can use:
`export MBX_HOME=$HOME/codes/MBX`

Then load the corresponding modules. 

If you compiled MBX with GNU:
`module load gcc-compilers`
`module load costinit`
`module load fftw3/gnu/3.3.5`

If you compiled MBX with Intel:
`module load gcc-compilers`
`module load intel-compilers`
`module load costinit`
`module load fftw3/intel/3.3.5`

Then go ahead and compile the driver:
`cd $MBX_HOME/plugins/i-pi/src/main`
`cp Makefile_dod_thunder Makefile`
Now make sure that the CXX compiler is set properly to intel or gnu, depending on how you compiled the MBX library
`make`
