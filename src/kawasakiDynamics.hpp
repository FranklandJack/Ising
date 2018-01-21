#ifndef kawasakiDynamics_hpp
#define kawasakiDynamics_hpp
#include <random>
#include "SpinLattice2D.hpp"
#include "metropolisUpdate.hpp"
void kawasakiDynamics(SpinLattice2D&,std::default_random_engine&,double, double, double);
#endif /* kawasakiDynamics_hpp */