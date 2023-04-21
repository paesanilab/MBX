# Description
These scripts will help you keep simulations running on THUNDER (DOD) without having to be on top of it all the time.

# Set up your simulations
A basic simulation will need some equilibration time and some production time. Usually, for our purposes, we use 100ps of equilibration and 1ns of production, but these times might vary as a function of what are you trying to calculate.

All the scripts that will be used are assuming a given folder tree:
```
|-- rerun_all.sh
|-- run.job
|-- run_next_eq.sh
|-- run_next_prod.sh
|-- system1
|   |-- run1
|   |   `-- eq1
|   |       |-- config.nrg
|   |       |-- config.xml
|   |       |-- config.xyz
|   |       |-- mbx.json
|   |       `-- run.job
|   |-- run2
|   |   `-- eq1
|   |       |-- config.nrg
|   |       |-- config.xml
|   |       |-- config.xyz
|   |       |-- mbx.json
|   |       `-- run.job
|   `-- run3
|       `-- eq1
|           |-- config.nrg
|           |-- config.xml
|           |-- config.xyz
|           |-- mbx.json
|           `-- run.job
`-- system2
    |-- run1
    |   `-- eq1
    |       |-- config.nrg
    |       |-- config.xml
    |       |-- config.xyz
    |       |-- mbx.json
    |       `-- run.job
    `-- run2
        `-- eq1
            |-- config.nrg
            |-- config.xml
            |-- config.xyz
            |-- mbx.json
            `-- run.job
```

Here, system1 and system2 are examples of two different simulations (different system, different temperatures...) These folders can have any name. It is recommended to call them by what they are, like `1na_in_277h2o_300K_1atm_npt` or stuff like this. 
The structure inside the system folders it must be like you see above. Each run will contain a different copy of the simulation initialized with a different seed, so sampling will be improved and done faster.
Inside the system folder there must be folders called `run1`, `run2`,... as many as you like, and each one of them must have an `eq1` folder with the inputs and the submission script. 

# Preparing the inputs
There are a couple things that one needs to be careful with:
1. Follow a pattern for the seed. In the config.xml file, the section `seed` defines the seed for the random initialization of the velocities and thermostats. If different systems have the same seed, that doesn't matter, but if different runs of the same system have the same seed, they will run exactly the same trajectory. As an example, these folders have a pattern for the seed. All the seeds start with `827`, and the last two numbers are the ones changing. For system1, the fourth number is `0`, and system2, `1`. The last number is the run number, so run1 of system1 will have a seed `82701`.
2. To avoid problems, follow the same pattern for the port number and the address (localhost). You can use the same three first numbers for the port of all your simulations and use the same strategy for the last two. Just as a note, the port number should be greater than 34600, and smaller than 99999. 

# Running the scripts
The main script that needs to be run is `rerun_all.sh`. In that script, modify `directories` to be all the directories in your folder that you want to keep running until completition. Then, make sure you submit each of the equilibration runs (`eq1`) directing the output to `jobid.dat`. THIS IS CRUCIAL.
`cd PATH/TO/EQ1`
`qsub run.job > jobid.dat`
A faster way, if you don;t wanna go manually:
```
for i in system1 system2; do cd $i; for j in `ls -d run*`; do cd $j/eq1 ; qsub run.job > jobid.dat ; cd ../../; done ; cd ../ ; done
```

Once that is done, will be enough to put the `rerun_all.sh` in a loop:
```
while [ 1 ]; do ./rerun_all.sh ; sleep 300; done
```
And come back after you went to the beach for a few hours to see how the simulations are doing :)

