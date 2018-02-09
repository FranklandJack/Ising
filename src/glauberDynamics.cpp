#include "glauberDynamics.hpp"
bool glauberDynamics(SpinLattice2D& spinLattice, 
					 std::default_random_engine &generator, 
					 double jConstant, 
					 double boltzmannConstant, 
					 double temperature)
{
	/*
	 * Create distributions to get a random spin.
	 * The arguments in the constructor give a closed interval so we have to subtract 1 off the maximum 
	 * in order to index an array.
	 */
	std::uniform_int_distribution<int> rowDistriubution(0,spinLattice.getRows()-1);
	std::uniform_int_distribution<int> colDistriubution(0,spinLattice.getCols()-1);

	// Draw the random spins.
	int row = rowDistriubution(generator);
	int col = colDistriubution(generator);

	// Calculate the energy associated to the spin to be flipped before flipping.
	double energyBefore = spinLattice.siteEnergy(row, col, jConstant);

	// Flip the spin.
	spinLattice.flip(row, col);

	// Calculate the energy after the flip.
	double energyAfter = spinLattice.siteEnergy(row, col, jConstant);

	/*
	 * Since the energy associated to the flip site are the only terms that contributes to the 
	 * sum in the change in energy the change in energy is just the difference between these 
	 * two energies.
	 */

	// Do the metropolis update.
	if (!metropolisUpdate(energyBefore, energyAfter, generator, boltzmannConstant, temperature))
	{	
		// Flip the spin back if the update failed.
		spinLattice.flip(row,col);

		// Tell the caller the metropolis update failed.
		return false;
	}

	// No need to flip the spin back if the update was successful.
	return true;
}