#ifndef kawasakiDynamics_hpp
#define kawasakiDynamics_hpp
#include <random>
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
bool kawasakiDynamics(SpinLattice2D &lattice, 
					  std::default_random_engine& generator,
					  double jConstant,
					  double boltzmannConstant,
					  double temperature);
#endif /* kawasakiDynamics_hpp */