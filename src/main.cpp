#include <iostream> // For file IO.
#include "SpinLattice2D.hpp" // For the spin lattice.
#include <boost/filesystem.hpp> // For constructing directories for file io.
#include <boost/program_options.hpp> // For command line arguments.
#include <fstream> // For file output.
#include <chrono> // For timing.
#include <ctime>  // For timing.
#include <random> // For generating random numbers.
#include "Timer.hpp" // For custom timer. 
#include <iomanip> // For output formatting.  

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

    /*************************************************************************************************************************
     ******************************************************** Input **********************************************************
     *************************************************************************************************************************/

    // Input parameters that user will specify.
    int rowCount, columnCount;
    double temperature;
    int burnPeriod;
    int measurementInterval;

    // Set up optional command line argument.
    po::options_description desc("Options for Ising model simulation");

    desc.add_options()
        // Option 'row-count' and 'r' are equivalent.
        ("row-count,r", po::value<int>(&rowCount)->default_value(50), "Number of rows in lattice.")
        // Option 'column-count' and 'c' are equivalent.
        ("column-count,c", po::value<int>(&columnCount)->default_value(50), "Number of columns on lattice.")
        // Option 'temperature' and 'T' are equivalent.
        ("temperature,T", po::value<double>(&temperature)->default_value(1), "Temperature of the lattice.")
        // Option 'burn-period' and 'b' are equivalent.
        ("burn-period,b", po::value<int>(&burnPeriod)->default_value(100), "Number of configurations before measurement starts.")
        // Option 'measurement-interval' and 'i' are equivalent.
        ("measurement-interval,i", po::value<int>(&measurementInterval)->default_value(10), "Number of configurations before measurement starts.")
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

    // Tell user their input values to check they are correct.
    int outputColumnWidth = 30;
    cout << "Input Parameters..." << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "# Rows: " << right << rowCount << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "# Columns : " << right << columnCount << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Temperature: " << right << temperature << '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Burn Period: " << right << burnPeriod<< '\n';
    cout << setw(outputColumnWidth) << setfill(' ') << left << "Measurement Interval: " << right << measurementInterval << '\n';


    // Create the lattice of spins.
    SpinLattice2D spinLattice{rowCount,columnCount};
    spinLattice.randomise(generator);
    cout << spinLattice;

    cout << setw(outputColumnWidth) << setfill(' ') << left << "Time take to execute(s):   " << right << timer.elapsed() << endl << endl;
    return 0;
}