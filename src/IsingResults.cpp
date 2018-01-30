#include "IsingResults.hpp"

std::ostream& operator<<(std::ostream &out, const IsingResults &results)
{	
	int outputColumnWidth = 30;
	out << "Results..." << '\n';
   	out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "E: " << std::right << results.energy << " +/- " << results.energyError << '\n';
	out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "M: " << std::right << results.magnetisation << " +/- " << results.magnetisationError << '\n';
	out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "X: " << std::right << results.susceptibility <<  " +/- " <<  results.susceptibilityError <<'\n';
	out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "C: " << std::right << results.heatCapacity << " +/- " << results.heatCapacityError << '\n';

	return out;
}