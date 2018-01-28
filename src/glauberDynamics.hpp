#ifndef glauberDynamics_hpp
#define glauberDynamics_hpp
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
#include <random>
bool glauberDynamics(SpinLattice2D&, std::default_random_engine&, double, double, double);
#endif /* glauberDynamics_hpp */