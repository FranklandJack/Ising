#include "jackKnife.hpp"
#include <iostream>

double jackKnife(const DataArray::IDataFunctor &fcn, const DataArray &data)
{	
	// Create data array to hold the means of the reduced sample sets.
	DataArray reducedMeans;
	reducedMeans.reserve(data.getSize());

	for(int i = 0; i < data.getSize(); ++i)
	{
		// Create temporary data array to hold the reduced sample set.
		DataArray tempDataArray;
		tempDataArray.reserve(data.getSize()-1);
		for(int j = 0; j < i; ++j)
		{
			tempDataArray.push_back(data[j]);
		}
		for(int j = i+1; j < data.getSize(); ++j)
		{
			tempDataArray.push_back(data[j]);
		}
		reducedMeans.push_back(tempDataArray.mean());
	}


	// Calculate error according to the formula for jack-knife.
	double mean 	  = reducedMeans.mean();
	double squareMean = reducedMeans.squareMean();
	double error = sqrt((squareMean - mean * mean) * data.getSize());

	return error;


}
