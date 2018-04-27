#include "kawasakiDynamics.hpp"
bool kawasakiDynamics(SpinLattice2D &spinLattice,
					  std::default_random_engine &generator,
					  double jConstant,
					  double boltzmannConstant,
					  double temperature)
{
	// Create random number generators for the rows and columns of the spin lattice
	// constructor range is closed upper bound so need to subtract 1.
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
	} while(row1==row2 && col1==col2);

	// Calculate energy before swap.
	double energyBefore = spinLattice.sitePairEnergy(row1, col1, row2, col2, jConstant);

	// Swap the spins.
	spinLattice.swap(row1, col1, row2, col2);

	// Calculate the energy after the swap.
	double energyAfter = spinLattice.sitePairEnergy(row1, col1, row2, col2, jConstant);

	// Do the metropolis update.
	if(!metropolisUpdate(energyBefore, energyAfter, generator, boltzmannConstant, temperature))
	{
		// If the update fails make sure we swap the spins back.
		spinLattice.swap(row1, col1, row2, col2);

		// And tell caller the update failed.
		return false;
	}

	// If successful no need to swap spins back.
	return true;

}
