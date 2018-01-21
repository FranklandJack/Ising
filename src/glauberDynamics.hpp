#ifndef glauberDynamics_hpp
#define glauberDynamics_hpp
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
#include <random>
void glauberDynamics(SpinLattice2D&, std::default_random_engine,double);
#endif /* glauberDynamics_hpp */