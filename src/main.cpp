#include <iostream> // For file IO.
#include "SpinLattice2D.hpp" // For the spin lattice.
#include "glauberDynamics.hpp" // For implementing the Glauber dynamics.
#include "kawasakiDynamics.hpp" //  For implementing the kawasaki dynamics.
#include <boost/filesystem.hpp> // For constructing directories for file io.
#include <boost/program_options.hpp> // For command line arguments.
#include <fstream> // For file output.
#include <chrono> // For timing.
#include <ctime>  // For timing.
#include <random> // For generating random numbers.
#include "Timer.hpp" // For custom timer. 
#include <iomanip> // For output formatting.
#include <cmath> // For any maths functions.  

namespace po = boost::program_options;
using namespace std;
int main(int argc, char const *argv[])
{
    /*************************************************************************************************************************
     ************************************************* Preparations **********************************************************
     *************************************************************************************************************************/

    // Start the clock so execution time can be calculated. 
    Timer timer;

    // Seed the pseudo random number generator using the system clock.
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());

    // Create a generator that can be fed to any distribution to produce pseudo random numbers according to that distribution. 
    default_random_engine generator(seed);

    // Begin a timer for the output directory.
    auto start = chrono::system_clock::now();

    // Create string from the time the program started.
    time_t startTime = chrono::system_clock::to_time_t(start);
    string outputName = ctime(&startTime);

    // Strip out and replace difficult characters.
    std::transform(outputName.begin(), outputName.end(), outputName.begin(), [](char ch) {return ch == ' ' ? '_' : ch;});
    std::transform(outputName.begin(), outputName.end(), outputName.begin(), [](char ch) {return ch == ':' ? '-' : ch;});
    outputName.erase(std::remove(outputName.begin(), outputName.end(), '\n'), outputName.end());

    // Create directory path from the string.
    boost::filesystem::path outPath = outputName;
    
    // If user calls program more than once a second so that directories will be overwritten append an index.
    for(int i = 2; boost::filesystem::exists(outPath) && i < 100; ++i)
    {
        stringstream ss;
        ss << outPath << "(" << i << ")";
        outPath = ss.str();
    }

    // Create the directory for output.
    boost::filesystem::create_directory(outPath);

    //Create output file for the spin array.
    ofstream spinsOutput(outputName+"/spins.dat",ios::trunc);

    /*************************************************************************************************************************
     ******************************************************** Input **********************************************************
     *************************************************************************************************************************/

    // Input parameters that user will specify.
    int rowCount, columnCount;
    double temperature;
    int burnPeriod;
    int measurementInterval;
    void (*dynamics)(SpinLattice2D&, default_random_engine&, double, double, double);
    double jConstant;
    int configurations;
    double boltzmannConstant;

    // Set up optional command line argument.
    po::options_description desc("Options for Ising model simulation");

    desc.add_options()
        // Option 'row-count' and 'r' are equivalent.
        ("row-count,r", po::value<int>(&rowCount)->default_value(50), "Number of rows in lattice.")
        // Option 'column-count' and 'c' are equivalent.
        ("column-count,c", po::value<int>(&columnCount)->default_value(50), "Number of columns on lattice.")
        // Option 'temperature' and 'T' are equivalent.
        ("temperature,T", po::value<double>(&temperature)->default_value(1), "Temperature of the lattice.")
        // Option 'glauber-dynamics' and 'g' are equivalent.
        ("glauber-dynamics,g", "Choice of Glauber dynamics (is also default).")
        // Option 'kawasaki-dynamics' and 'k' are equivalent.
        ("kawasaki-dynamics,k", "Choice of Kawasaki Dynamics (will take precedence if user specialized Glauber dynamics as well).")
        // Option 'J-constant' and 'J' are equivalent.
        ("J-constant,J", po::value<double>(&jConstant)->default_value(1), "J constant that determines units of energy.")
        // Options 'Boltzmann-constant' and 'B' are equivalent.
        ("Boltzmann-constant",po::value<double>(&boltzmannConstant)->default_value(1),"Boltmann constant.")
        // Option 'configurations' and 'C' are equivalent.
        ("configurations,C", po::value<int>(&configurations)->default_value(100000),"Number of configurations to generate after the burn period.")
        // Option 'burn-period' and 'b' are equivalent.
        ("burn-period,b", po::value<int>(&burnPeriod)->default_value(100), "Number of configurations before measurement starts.")
        // Option 'measurement-interval' and 'i' are equivalent.
        ("measurement-interval,i", po::value<int>(&measurementInterval)->default_value(10), "How many configurations between measurement are made.")
        // Option 'help' and 'h' are equivalent.
        ("help,h", "produce help message");

    po::variables_map vm;
    po::store(po::parse_command_line(argc,argv,desc), vm);
    po::notify(vm);

    // If the user asks for help display it then exit.
    if(vm.count("help"))
    {
        cout << desc << "\n";
        return 1;
    }

    // Set the dynamics pointer to point at the right dynamics function.
    // Default to Glauber - even if Glauber specified this will work/
    dynamics = glauberDynamics;

    // Check if Kawasaki specified, if so this takes precedence.
    if(vm.count("kawasaki-dynamics"))
    {
    	dynamics = kawasakiDynamics;
    }



    // Tell user their input values to check they are correct.
    int outputColumnWidth = 30;
    cout << "Input Parameters..." << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "# Rows: " << right << rowCount << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "# Columns : " << right << columnCount << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Dynamics: " << right << (vm.count("kawasaki-dynamics") ? "Kawasaki" : "Glauber") << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Temperature: " << right << temperature << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "J: " << right << jConstant << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "K_B: " << right << boltzmannConstant << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Burn Period: " << right << burnPeriod<< '\n';
	cout << setw(outputColumnWidth) << setfill(' ') << left << "# Configurations: " << right << configurations<< '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Measurement Interval: " << right << measurementInterval << '\n';


    // Create the lattice of spins.
    SpinLattice2D spinLattice{rowCount,columnCount};
    spinLattice.randomise(generator);
    
    // Create the output variables that will hold the Monte-Carlo estimates.
    double totalEnergy{0};
    double totalEnergySquared{0};
    double totalMagnetisation{0};
    double totalMagnetisationSquared{0};

    // Main loop that actually runs the simulation.
    for(int config = 0; config < burnPeriod+configurations; ++config)
    {
    	// Update system.
    	dynamics(spinLattice, generator, jConstant, boltzmannConstant, temperature);

    	// If we are out of the burn period and on a measurement configuration then make any measurements.
    	if((config>burnPeriod) && ((config&measurementInterval) == 0))
    	{
    		double energy = spinLattice.latticeEnergy(jConstant);
    		totalEnergy += energy;
    		totalEnergySquared += energy*energy;

    		double magnetisation = spinLattice.totalMag();
    		totalMagnetisation += magnetisation;
    		totalMagnetisation += magnetisation*magnetisation;

    		spinsOutput << spinLattice;
    	}
    }

    // Make sure Monte-Carlo estimates have been averaged.
    int totalSamples = configurations/measurementInterval;
    totalEnergy 	   		  /= totalSamples;
    totalEnergySquared 		  /= totalSamples;
    totalMagnetisation 		  /= totalSamples;
    totalMagnetisationSquared /= totalSamples;

    // Calculate the standard error in the measured values.
    double totalEnergySDE = sqrt((totalEnergySquared - totalEnergy * totalEnergy)/totalSamples);
    double totalMagnetisationSDE = sqrt((totalMagnetisationSquared - totalMagnetisation * totalMagnetisation)/totalSamples);

    // Output results to command line.
    cout << "Output..." << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "<E> = " << right << totalEnergy << " +/- " << totalEnergySDE << '\n';
	cout << setw(outputColumnWidth) << setfill(' ') << left << "<M> = " << right << totalMagnetisation << " +/- " << totalMagnetisationSDE << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Time take to execute(s) =    " << right << timer.elapsed() << endl << endl;
    return 0;
}