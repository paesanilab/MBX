# Installation of PLUMED
Download PLUMED from the official webpage:
https://www.plumed.org/

The version tested is PLUMED v2.5.3

After the source code is extracted, follow this procedure:
1. Go to the plumed folder you just unpacked
2. Type `./configure --prefix=$HOME/codes/plumed-2.5.3 --enable-python`
3. Then, `make` and afterwards `make install`
4. Once it is done, do `pip2.7 install --user plumed` (since we will use it with i-pi, which uses python 2.7)

# Usage
Everytime that you want to use plumed, you need to set up the following variables:
`export PLUMED_KERNEL=PATH/TO/PLUMED/HOME/lib/libplumedKernel.so`
`source sourceme.sh`

Now PLUMED should be working and talking to i-pi. Look at the examples in this folder.
