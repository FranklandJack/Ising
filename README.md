# Ising
Simulation of Ising model for a 2D spin lattice

## Outline 

This program simulates a 2D [Ising model](https://en.wikipedia.org/wiki/Ising_model) using Monte-Carlo methods. The lattice consists of binary spins, however, spin values are accessed through a look up table and the model should therefore generalise easily to deal with an arbitrary spin which can take 2,3,4... etc values.

## Build Instructions

- To build the code run: ```$ make``` in the main directory. 
- For a full list of make functionality run ```$ make help```. 
- Note: The program makes use of the [Boost Library](http://www.boost.org/) for optional command line arguments and for creating output directories (because of this functionality the code may not compile on Windows systems), either way the boost library will need to be installed for the code to compile.
- Note: The program also makes extensive use of the C++11 random library as well as some other C++11 features. It will therefore need to be compiled with a compiler that has the C++11 standard.

## Running Instructions

Lots of options are customizable when running the code, although if no values are chosen sensible values will be used by default.
- For a full list of optional command line arguments and their default values run: ```$ ./ising --help``` or ```./ising -h```.
The user can also set an optional output directory using this method.

### Data Analysis

Although the executable runs at a single temperature and outputs the energy, magnetisation, susceptibility and heat capacity of the system it is often desirable to run the simulation at a range of temperatures. In order to do this simply run the bash script written for this purpose by running ```$ ./temperatureRuns``` which will run the program at a range of temperatures at an increment which are all customizable from within the script. This script makes use of Python to get floating points values in a bash script so Python will need to be installed. 

If you wish to collate data for a range of temperatures this can also be done with a dedicated bash script. First move all the output directories containing the data you wish to collate into a single parent directory. Then run ```$ ./collate``` on that directory. This will produce four .dat files for the quantities of interest e.g. the Temperature-Heat Capacity file will be TC.dat. These can be easily plotted using gnuplot via the dedicated script; simply run ```$ gnuplot plots``` which will construct four .png files containing the graphs. [Gnuplot](http://www.gnuplot.info/) will need to be installed for this functionality to work.

### Animation

The program currently has an extremely crude method for animating the simulation. If the ```./ising -a``` flag is provided when the program is run the state of the lattice will be printed to a file called spins.dat in the output directory every 10 sweeps. This can then be animated if the gnuplot script animate.gp is run simultaneously. To do this run ```./ising -a -o OutputDirectory``` in one bash shell, then in another run ```gnuplot -e "filename='OutputDirectory/spins.dat" animate.gp```. This method of animation is not efficient and needs to be replaced with calls to a graphics library from within the code itself.