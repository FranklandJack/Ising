#ifndef metropolisUpdate_hpp
#define metropolisUpdate_hpp
#include <random>
#include <cmath>
bool metropolisUpdate(double energyBefore, 
					  double ernegyAfter, 
					  std::default_random_engine& generator, 
					  double boltzmannConstant, 
					  double temperature);
#endif /* metropolisUpdate_hpp */