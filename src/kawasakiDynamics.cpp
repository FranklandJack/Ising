#include "kawasakiDynamics.hpp"
bool kawasakiDynamics(SpinLattice2D& spinLattice, std::default_random_engine &generator, double jConstant, double boltzmannConstant, double temperature)
{
	// Create random number genereators for the rows and columns of the spin lattice.
	std::uniform_int_distribution<int> rowDistriubution(0,spinLattice.getRows()-1);
	std::uniform_int_distribution<int> colDistriubution(0,spinLattice.getCols()-1);

	// Generate the coordinates of the first lattice site.
	int row1 = rowDistriubution(generator);
	int col1 = colDistriubution(generator);

	// Coordinates for second lattice site.
	int row2, col2;

	// Generate the second lattice site making sure it is different from the first.
	do
	{
		row2 = rowDistriubution(generator);
		col2 = colDistriubution(generator);
	} while(row1==row2 || col1==col2);

	// Forward declare the energies since they will need to be calculate in different ways depending on choice
	// of rows and columns.
	double energyBefore = spinLattice.sitePairEnergy(row1, col1, row2, col2, jConstant);
	spinLattice.swap(row1, col1, row2, col2);
	double energyAfter = spinLattice.sitePairEnergy(row1, col1, row2, col2, jConstant);
	if(!metropolisUpdate(energyBefore, energyAfter, generator, boltzmannConstant, temperature))
	{
		spinLattice.swap(row1, col1, row2, col2);
		return false;
	}

	return true;
	
}