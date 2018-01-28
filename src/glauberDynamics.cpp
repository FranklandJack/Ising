#include "glauberDynamics.hpp"
bool glauberDynamics(SpinLattice2D& spinLattice, std::default_random_engine &generator, double jConstant, double boltzmannConstant, double temperature)
{
	std::uniform_int_distribution<int> rowDistriubution(0,spinLattice.getRows()-1);
	std::uniform_int_distribution<int> colDistriubution(0,spinLattice.getCols()-1);

	int row = rowDistriubution(generator);
	int col = colDistriubution(generator);

	double energyBefore = spinLattice.siteEnergy(row, col, jConstant);
	spinLattice.flip(row, col);
	double energyAfter = spinLattice.siteEnergy(row, col, jConstant);
	if (!metropolisUpdate(energyBefore, energyAfter, generator, boltzmannConstant, temperature))
	{
		spinLattice.flip(row,col);
		return false;
	}
	return true;
}