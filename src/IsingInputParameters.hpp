#ifndef IsingInputParameters_hpp
#define IsingInputParameters_hpp
#include <iostream>
#include <iomanip>
class IsingInputParameters 
{
public:
	enum DynamicsType
	{
		Glauber,
		Kawasaki,

	};
	int rowCount;
	int columnCount;
    double temperature;
    int burnPeriod;
    int measurementInterval;
    DynamicsType dynamics;
    double jConstant;
    double boltzmannConstant;
    int sweeps;

    friend std::ostream& operator<<(std::ostream& out, const IsingInputParameters& params);

};
#endif /* IsingInputParameters_hpp */