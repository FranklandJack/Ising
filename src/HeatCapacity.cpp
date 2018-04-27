#include "HeatCapacity.hpp"

HeatCapacity::HeatCapacity(double boltzmannConstant, double temperature) : 	m_boltzmannConstant(boltzmannConstant),
																			m_temperature(temperature){}


double HeatCapacity::operator()(const DataArray &data) const
{
	return 1/(m_boltzmannConstant * m_temperature * m_temperature) * (data.variance()); 
}
