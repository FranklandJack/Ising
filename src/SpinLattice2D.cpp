#include "SpinLattice2D.hpp"

SpinLattice2D::SpinLattice2D(int columns, int rows): m_colCount(columns), m_rowCount(rows)
{
	m_spinMatrix.resize(columns*rows);
}

int& SpinLattice2D::operator()(int row, int col)
{
	return m_spinMatrix[col + row * m_colCount];
}

const int& SpinLattice2D::operator()(int row, int col) const
{
	return m_spinMatrix[col + row * m_colCount];
}


void SpinLattice2D::randomise(std::default_random_engine generator)
{
	std::bernoulli_distribution distribution(0.5);
	for(auto& spin : m_spinMatrix)
	{
		spin = (distribution(generator)? 1 : -1);
	}
}

std::ostream& operator<<(std::ostream& out, const SpinLattice2D& spinLattice)
{
	for(unsigned row = 0; row < spinLattice.m_rowCount; ++row)
	{
		for(unsigned col = 0; col < spinLattice.m_colCount; ++col)
		{
			out << spinLattice(row,col) << ' ';
		}
		out << '\n';
	}
	return out;
}

void SpinLattice2D::flip(int row, int col)
{
	(*this)(row,col) *= -1;
}

void SpinLattice2D::swap(int row1, int col1, int row2, int col2)
{
	std::swap((*this)(row1,col1),(*this)(row2,col2));
}

double SpinLattice2D::siteEnergy(int row, int col, double jConstant) const 
{
	double sum = 0;
	sum += (*this)(row,col) * (*this)(row+1,col);
	sum += (*this)(row,col) * (*this)(row-1,col);
	sum += (*this)(row,col) * (*this)(row,col+1);
	sum += (*this)(row,col) * (*this)(row,col-1);
	return sum * jConstant;
}

double SpinLattice2D::latticeEnergy(double jConstant) const
{
	double sum = 0;
	for(unsigned row = 0; row < m_rowCount; ++row)
	{
		for(unsigned col = 0; col < m_colCount; ++col)
		{
			sum += (*this)(row,col) * (*this)(row,col+1);
			sum += (*this)(row,col) * (*this)(row+1,col);
		}
	}

	return jConstant*sum;
}