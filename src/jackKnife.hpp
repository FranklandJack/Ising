#ifndef jackKnife_hpp
#define jackKnife_hpp
#include "DataArray.hpp"

double jackKnife(const DataArray::IDataFunctor &fcn, const DataArray &data);

#endif /* jackKnife_hpp */