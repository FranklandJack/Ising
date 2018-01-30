#ifndef IsingResults_hpp
#define IsingResults_hpp
#include <iostream>
#include <iomanip>

class IsingResults
{
public:

	double energy;
	double energyError;

	double magnetisation;
	double magnetisationError;

	double susceptibility;
	double susceptibilityError;

	double heatCapacity;
	double heatCapacityError;

	friend std::ostream& operator<<(std::ostream& out, const IsingResults &results);
};

#endif /* IsingResults_hpp */
