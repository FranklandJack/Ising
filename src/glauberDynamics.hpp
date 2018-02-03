#ifndef glauberDynamics_hpp
#define glauberDynamics_hpp
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
#include <random>
bool glauberDynamics(SpinLattice2D& lattice, 
					 std::default_random_engine& generator, 
					 double jConstant, 
					 double boltzmannConstant, 
					 double temperature);
#endif /* glauberDynamics_hpp */