#ifndef glauberDynamics_hpp
#define glauberDynamics_hpp
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
#include <random>
/**
 *\file
 *\brief function perform Glauber dynamics on the array.
 *\param lattice a SpinLattice2D reference that has the dynamics performed on it.
 *\param generator an std::default_random_engine reference used for the random number generation in the call
 * to the metropolis update and choosing the spin to flip.
 *\param jConstant floating point value representing the J constant in the energy expression for a 2D spin lattice.
 *\param boltzmannConstant floating point value representing the Boltzmann constant
 *\param temperature floating point value representing the temperature of the system.
 *\return boolean value representing whether the update was successful.
 *
 * The Glayber dynamics work by proposing a new configuration by flipping a spin and performing a Metropolis
 * update.
 */
bool glauberDynamics(SpinLattice2D& lattice,
					 std::default_random_engine& generator,
					 double jConstant,
					 double boltzmannConstant,
					 double temperature);
#endif /* glauberDynamics_hpp */
