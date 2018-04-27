#ifndef kawasakiDynamics_hpp
#define kawasakiDynamics_hpp
#include <random>
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
/**
 *\file
 *\brief function perform Kawasaki dynamics on the array.
 *\param lattice a SpinLattice2D reference that has the dynamics performed on it.
 *\param generator an std::default_random_engine reference used for the random number generation in the call
 * to the metropolis update and choosing the spins to flip.
 *\param jConstant floating point value representing the J constant in the energy expression for a 2D spin lattice.
 *\param boltzmannConstant floating point value representing the Boltzmann constant
 *\param temperature floating point value representing the temperature of the system.
 *\return boolean value representing whether the update was successful.
 *
 * The Kawasaki dynamics work by proposing a new configuration by swapping 2 spins and performing a metropolis
 * update.
 */
bool kawasakiDynamics(SpinLattice2D &lattice,
					  std::default_random_engine& generator,
					  double jConstant,
					  double boltzmannConstant,
					  double temperature);
#endif /* kawasakiDynamics_hpp */
