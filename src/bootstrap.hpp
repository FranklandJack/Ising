#ifndef bootstrap_hpp
#define bootstrap_hpp

#include "DataArray.hpp"
#include "IDataFunctor.hpp"
#include <random>
#include <cmath>


double bootstrap(const DataArray::IDataFunctor &fcn, const DataArray &data, std::default_random_engine generator, int iterations = 100);

#endif /* bootstrap_hpp */