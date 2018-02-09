#include <iostream> // For file IO.
#include "SpinLattice2D.hpp" // For the spin lattice.
#include "glauberDynamics.hpp" // For implementing the Glauber dynamics.
#include "kawasakiDynamics.hpp" //  For implementing the kawasaki dynamics.
#include "SpinBitLattice2D.hpp" // For optimised implementation of spin lattice.
#include "DataArray.hpp" // For holding Monte-Carlo samples and easily calculating their means and errors.
#include "makeDirectory.hpp" // For creating output directories.
#include "getTimeStamp.hpp" // For getting a time stamp.
#include "IsingInputParameters.hpp" // For easily printing input variables to any output stream.
#include "IsingResults.hpp" // For easily printing numerical results to any output stream.
#include "HeatCapacity.hpp" // For calculating heat capacity.
#include "Susceptibility.hpp" // For calculating susceptibility.
#include "jackKnife.hpp" // For doing jack-knife errors.
#include "bootstrap.hpp" // For doing bootstrap errors.
#include <boost/filesystem.hpp> // For constructing directories for file IO.
#include <boost/program_options.hpp> // For command line arguments.
#include <fstream> // For file output.
#include <chrono> // For timing.
#include <ctime>  // For timing.
#include <random> // For generating random numbers.
#include "Timer.hpp" // For custom timer. 
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

/*************************************************************************************************************************
******************************************************** Input **********************************************************
*************************************************************************************************************************/

    // Input parameters that user will specify.
    int rowCount, columnCount;
    double temperature;
    int burnPeriod;
    int measurementInterval;
    bool (*dynamics)(SpinLattice2D&, default_random_engine&, double, double, double);
    IsingInputParameters::ErrorTypes errorMethod;
    IsingInputParameters::DynamicsType dynamicsType;
    double jConstant;
    double boltzmannConstant;
    bool outputLattice;
    int sweeps;
    int autoCorrelationRange;
    string outputName;

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
        // Option 'sweeps' and 's' are equivalent.
        ("sweeps,s", po::value<int>(&sweeps)->default_value(10000),"Number of sweeps to generate after the burn period.")
        // Option 'burn-period' and 'b' are equivalent.
        ("burn-period,b", po::value<int>(&burnPeriod)->default_value(100), "Number of sweeps before measurement starts.")
        // Option 'measurement-interval' and 'i' are equivalent.
        ("measurement-interval,i", po::value<int>(&measurementInterval)->default_value(10), "How many sweeps between measurement are made.")
        // Option 'autocorrrelation-range' and 'C' are equivalent.
        ("autocorrelation-range,C", po::value<int>(&autoCorrelationRange)->default_value(100), "Range of autocorrelation function for output.")
        // Option 'output' and 'o' are equivalent.
        ("output,o",po::value<std::string>(&outputName)->default_value(getTimeStamp()), "Name of output directory to save output files into.")
        // 'bootstrap is the only option'
        ("bootstrap", "Use bootstrap method to calculate errors that depend on second central moment (default)")
        // 'jackknife only option'
        ("jackknife","Use jackknife meothod to calculate the errors that depend on second moment (takes precedence if selected")
        // Option 'animate' and 'a' are equivalent.
        ("animate,a","Output the lattice after each sweep for animation.")
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
    dynamicsType = IsingInputParameters::Glauber;

    // Check if Kawasaki specified, if so this takes precedence.
    if(vm.count("kawasaki-dynamics"))
    {
    	dynamics = kawasakiDynamics;
    	dynamicsType = IsingInputParameters::Kawasaki;
    }

    // By default don't output the lattice
    outputLattice = false;
    // If the user specified lattice output then make sure it happens. 
    if(vm.count("animate"))
    {
    	outputLattice = true;
    }
    
    // By defualt use bootstrap.
    errorMethod = IsingInputParameters::Bootstrap;

    // If the user specified error calculation method then use that
    if(vm.count("jackknife"))
    {
    	errorMethod = IsingInputParameters::JackKnife;
    }

    // Construct an input parameter object, this just makes printing a lot cleaner.
    IsingInputParameters inputParameters
    {
    	rowCount, 
    	columnCount, 
    	temperature, 
    	burnPeriod, 
		measurementInterval, 
		dynamicsType, 
		jConstant,
		boltzmannConstant,
		sweeps,
		autoCorrelationRange,
        errorMethod
	};

/*************************************************************************************************************************
************************************************* Create Output Files ***************************************************
*************************************************************************************************************************/
    
    // Create an output directory from either the default time stamp or the user defined string.
    makeDirectory(outputName);

    //Create output file for the spin array.
    fstream spinsOutput(outputName+"/spins.dat",ios::out);

    // Create output file for the input parameters.
    fstream inputParameterOutput(outputName+"/input.txt",ios::out);

    // Create an output file for the output values.
    fstream resultsOutput(outputName+"/results.txt",ios::out);

    // Create an output file for the energy. 
    fstream energyDataOutput(outputName+"/energy.dat",ios::out);

    // Create an output file for the magnetisation.
    fstream magnetisationDataOutput(outputName + "/magentistaion.dat", ios::out);

    // Create an output file for the autocorrelation of the magnetisation.
    fstream magnetisationAutoCorrelationOutput(outputName + "/magnetisationAutocorrelation.dat", ios::out);

    // Create an output file for the auto-correlation of the energy.
    fstream energyAutoCorrelationOutput(outputName + "/energyAutoCorrelation.dat", ios::out);

    // Create an output file for initial configuration.
    fstream initialConfigOutput(outputName + "/initalConfiguration.dat", ios::out);

/*************************************************************************************************************************
************************************************* Simulation Set Up *****************************************************
*************************************************************************************************************************/

    // Print input parameters to command line.
    cout << inputParameters << '\n';

    // Print input parameters to an output file 
    inputParameterOutput << inputParameters << '\n';
   
    // Create the lattice of spins.
    SpinLattice2D spinLattice(rowCount,columnCount);

    // Set lattice if Kawasaki dynamics is being used, otherwise keep it aligned.
    if(vm.count("kawasaki-dynamics"))
    {
        // If the temperature is low we can set the lattice in the ground state.
        if(temperature < 1.5)
        {
            spinLattice.setEvenSpins();
        }

        // Otherwise have it random.
        else
        {
            spinLattice.randomise(generator);
        }
    }
    initialConfigOutput << spinLattice;
    
    // Work out the number of samples we need.
    int totalSamples = sweeps/measurementInterval;

    // Create the output variables that will hold the Monte-Carlo estimates. 
    int totalSites = spinLattice.getSize();

    // Since we know how many samples we will take faster to reserve the space before hand.
    DataArray energyData;
    energyData.reserve(totalSamples);

    DataArray magnetisationData;
    magnetisationData.reserve(totalSamples);

    DataArray scaledMagnetisation;
    scaledMagnetisation.reserve(totalSamples);

/*************************************************************************************************************************
************************************************* The Simulation ********************************************************
*************************************************************************************************************************/

    // Main loop that actually runs the simulation.
    for(int sweep = 0; sweep < burnPeriod+sweeps; ++sweep)
    {
    	for(int site = 0; site < totalSites; ++site)
    	{
            dynamics(spinLattice, generator, jConstant, boltzmannConstant, temperature);
    	}
    		
    	// If we are out of the burn period and on a measurement sweep then make any measurements.
    	if((sweep >= burnPeriod) && ((sweep % measurementInterval) == 0))
    	{
    		double sweepEnergy = spinLattice.latticeEnergy(jConstant);
    		energyData.push_back(sweepEnergy);

    		double sweepMagnetistation = spinLattice.totalMag();
    		magnetisationData.push_back(abs(sweepMagnetistation));	
    	}

        // If the user plans to animate the configuration then output it here.
        if(outputLattice && ((sweep % measurementInterval) == 0))
        {
            spinsOutput.seekg(0,ios::beg);
            spinsOutput << spinLattice << flush;
        }
    }

/*************************************************************************************************************************
****************************************************  Analysis **********************************************************
*************************************************************************************************************************/

    // Print the final configuration so it can be reused in future.
    spinsOutput.seekg(0,ios::beg);
    spinsOutput << spinLattice << flush;
    

   	// Print data to files.
   	energyDataOutput << energyData;
   	magnetisationDataOutput << magnetisationData;

   	// Calculate the auto-correlation in the magnetisation and energy and print it.
   	std::vector<double> magAutoCorrelation = magnetisationData.autoCorrelation(0,autoCorrelationRange);
   	std::vector<double> engAutoCorrelation = energyData.autoCorrelation(0,autoCorrelationRange);
   	for(int i = 0; i < magAutoCorrelation.size(); ++i)
   	{
   		magnetisationAutoCorrelationOutput << i << ' ' << magAutoCorrelation[i] << '\n';
   		energyAutoCorrelationOutput << i << ' ' << engAutoCorrelation[i] << '\n';
   	}

   	// Calculate any numerical values we need and their errors.
   	double energy 	   = energyData.mean();
   	double energyError = energyData.error();

   	double magnetisation 	  = magnetisationData.mean();
   	double magnetisationError = magnetisationData.error();

   	// Construct the susceptibility functor.
   	Susceptibility susceptibilityFcn(boltzmannConstant, temperature);

   	double susceptibility = susceptibilityFcn(magnetisationData)/(spinLattice.getSize());
	double errorSusceptibility;
	switch (errorMethod)
	{
		case IsingInputParameters::Bootstrap : errorSusceptibility = bootstrap(susceptibilityFcn, magnetisationData, generator)/(spinLattice.getSize()); 
						 break;

		case IsingInputParameters::JackKnife : errorSusceptibility = jackKnife(susceptibilityFcn, magnetisationData)/(spinLattice.getSize());
						 break;
	}

   	// Construct the heat capacity functor. 
   	HeatCapacity heatCapacityFcn(boltzmannConstant, temperature);

   	double heatCapacity = heatCapacityFcn(energyData)/(spinLattice.getSize());
   	double errorHeatCapacity;
   	switch (errorMethod)
	{
		case IsingInputParameters::Bootstrap : errorHeatCapacity = bootstrap(heatCapacityFcn, energyData, generator)/(spinLattice.getSize()); 
						 			 break;

		case IsingInputParameters::JackKnife : errorHeatCapacity = jackKnife(heatCapacityFcn, energyData)/(spinLattice.getSize());
						 			 break;
	}
    
    // Construct object that holds results.
   	IsingResults results
   	{
   		energy, 
   		energyError, 
   		magnetisation,
   		magnetisationError,
   		susceptibility,
   		errorSusceptibility,
   		heatCapacity,
   		errorHeatCapacity 
   	};

/*************************************************************************************************************************
***********************************************  Output/Clean Up ********************************************************
*************************************************************************************************************************/

   	// Output results to file.
   	resultsOutput << results << '\n';

    // Output results to command line.
    cout << results << '\n';

    // Report how long the program took to execute.
    cout << setw(30) << setfill(' ') << left << "Time take to execute(s) =    " << right << timer.elapsed() << endl << endl;

    return 0;
}