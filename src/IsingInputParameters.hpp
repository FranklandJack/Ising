#ifndef IsingInputParameters_hpp
#define IsingInputParameters_hpp
#include <iostream>
#include <iomanip>
/**
 *\file 
 *\class IsingInputParameters
 *\brief Class for easily handling input parameters of Ising Monte-Carlo method.
 *
 * This class essentially just holds some values and has an operator to easily output
 * them to a stream.
 */
class IsingInputParameters 
{
public:
	/**
	 *\enum DynamicsType.
	 *\brief represents the two possible types of dynamics.
	 */
	enum DynamicsType
	{
		Glauber,
		Kawasaki,

	};
	/**
	 *\enum ErrorTypes.
	 *\brief represents the two methods of error calculation.
	 */
	enum ErrorTypes
	{
		Bootstrap,
		JackKnife,
	};
	/// Number of rows in lattice.
	int rowCount;
	/// Number of columns in lattice.
	int columnCount;
	/// Temperature of system.
    double temperature;
    /// Number of sweeps until results start being recorded.
    int burnPeriod;
    /// Sweep interval at which measurements are made.
    int measurementInterval;
    /// The type of dynamics used.
    DynamicsType dynamics;
    /// Value of the J constant.
    double jConstant;
    /// Value of the Boltzmann constant.
    double boltzmannConstant;
    /// Total number of sweeps in the simulation.
    int sweeps;
    /// The range for which to calculate the autocorrelation for.
    int autoCorrelationRange;
    /// The method used for calculating more complicated errors.
    ErrorTypes errorType;

    /** 
	 *\brief operator<< overload for outputting the results.
	 *\param out std::ostream reference that is the stream being outputted to.
	 *\param params constant IsingInputParameters instance to be output.
	 *\return std::ostream reference so the operator can be chained.
	 *
	 * Results will be output in a formatted table for easy viewing in the command line or a file.
	 */
    friend std::ostream& operator<<(std::ostream& out, const IsingInputParameters& params);

};
#endif /* IsingInputParameters_hpp */