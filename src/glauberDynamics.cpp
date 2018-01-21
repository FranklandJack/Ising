#include "glauberDynamics.hpp"
void glauberDynamics(SpinLattice2D& spinLattice, std::default_random_engine generator, double jConstant)
{
	std::uniform_int_distribution<int> rowDistriubution(0,spinLattice.getRows());
	std::uniform_int_distribution<int> colDistriubution(0,spinLattice.getCols());

	int row = rowDistriubution(generator);
	int col = colDistriubution(generator);

	double energyBefore = spinLattice.siteEnergy(row, col, jConstant);
	spinLattice.flip(row, col);
	double energyAfter = spinLattice.siteEnergy(row, col, jConstant);
	if (!metropolisUpdate(energyBefore, energyAfter, generator))
		spinLattice.flip(row,col);
}