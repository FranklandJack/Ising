#include "Susceptibility.hpp"

Susceptibility::Susceptibility(double boltzmannConstant, double temperature) : m_boltzmannConstant(boltzmannConstant),
																			 m_temperature(temperature) {}

double Susceptibility::operator()(const DataArray &data) const
{
	return 1/(m_boltzmannConstant * m_temperature) * (data.squareMean() - data.mean() * data.mean());
}