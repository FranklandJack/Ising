#ifndef metropolisUpdate_hpp
#define metropolisUpdate_hpp
#include <random>
#include <cmath>
/**
 *\file
 *\brief function to perform metropolis update.
 *\param energyBefore floating point value representing the energy before the proposed update.
 *\param energyAfter floating point value representing the energy after the proposed update.
 *\param generator default_random_engine reference used for generating the random numbers in the metropolis update.
 *\param boltzmannConstant floating point value representing the boltzmann constant.
 *\param temperature floating point value representing the temperature of the system.
 *\return boolean value representing whether the proposed update should be accepted or rejected.
 */
bool metropolisUpdate(double energyBefore, 
					  double ernegyAfter, 
					  std::default_random_engine& generator, 
					  double boltzmannConstant, 
					  double temperature);
#endif /* metropolisUpdate_hpp */