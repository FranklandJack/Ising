#ifndef Susceptibility_hpp
#define Susceptibility_hpp
#include "DataArray.hpp"

class Susceptibility : public DataArray::IDataFunctor
{
private:
	double m_boltzmannConstant;
	double m_temperature;

public:
	Susceptibility(double boltzmannConstant, double temperature);

	double operator()(const DataArray &data) const;

};

#endif /* Susceptibility_hpp */