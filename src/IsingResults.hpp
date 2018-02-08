#ifndef IsingResults_hpp
#define IsingResults_hpp
#include <iostream>
#include <iomanip>

/**
 *\file 
 *\class IsingResults
 *\brief Class for easily handling results of Ising Monte-Carlo method.
 *
 * This class essentially just holds some values and has an operator to easily output
 * them to a stream.
 */
class IsingResults
{
public:
	/// Lattice energy.
	double energy;
	/// Error in lattice energy.
	double energyError;

	/// Lattice magnetisation.
	double magnetisation;
	/// Error in lattice magnetisation.
	double magnetisationError;

	/// Lattice susceptibility.
	double susceptibility;
	/// Error in lattice susceptibility.
	double susceptibilityError;

	/// Lattice heat capacity.
	double heatCapacity;
	/// Error in lattice heat capacity.
	double heatCapacityError;

	/** 
	 *\brief operator<< overload for outputting the results.
	 *\param out std::ostream reference that is the stream being outputted to.
	 *\param results constant IsingResults instance to be output.
	 *\return std::ostream reference so the operator can be chained.
	 *
	 * Results will be output in a formatted table for easy viewing in the command line or a file.
	 */
	friend std::ostream& operator<<(std::ostream& out, const IsingResults &results);
};

#endif /* IsingResults_hpp */
