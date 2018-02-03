#ifndef HeatCapacity_hpp
#define HeatCapacity_hpp
#include "DataArray.hpp"

class HeatCapacity : public DataArray::IDataFunctor
{
private:
	double m_boltzmannConstant;
	double m_temperature;
public:
	HeatCapacity(double boltzmannConstant, double temperature);
	virtual double operator()(const DataArray &data) const override;
};

#endif /* HeatCapacity_hpp */