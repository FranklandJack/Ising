#ifndef DataArray_hpp
#define DataArray_hpp

#include <vector>
#include <cmath>
#include <random>
#include <iostream>


class DataArray
{
private:
	std::vector<double> m_data;
	int m_size;

public:
	DataArray();
	DataArray(int);

	double& operator[](int);
	const double& operator[](int) const;

	void push_back(double);
	void pop_back();

	void reserve(int);

	double mean() const;
	double squareMean() const;
	double error() const;

	double jackKnifeMean() const;
	double jackKnifeError() const;

	double bootstrapMean() const;
	double boostrapError() const;

	double sum() const;

	friend std::ostream& operator<<(std::ostream&, const DataArray&);

	double autoCorrelation(int) const;

	std::vector<double> autoCorrelation(int,int) const;

	void printAutoCorrelation(int,int) const;




};

#endif /* DataArray_hpp */