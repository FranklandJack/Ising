#include "SpinLattice2D.hpp"

SpinLattice2D::SpinLattice2D(int rows, int cols): m_rowCount{rows}, m_colCount{cols}, m_spinMatrix(m_rowCount*m_colCount,1)
{
}

int& SpinLattice2D::operator()(int row, int col)
{
	row %= m_rowCount;
	col %= m_colCount;
	return m_spinMatrix[col + row * m_colCount];
}

const int& SpinLattice2D::operator()(int row, int col) const
{
	row %= m_rowCount;
	col %= m_colCount;
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
	for(int row = 0; row < spinLattice.m_rowCount; ++row)
	{
		for(int col = 0; col < spinLattice.m_colCount; ++col)
		{
			out << std::showpos << spinLattice(row,col) << ' ';
		}
		if(row != spinLattice.m_rowCount-1)
		{
			out << '\n';
		}
	}
	return out;
}

void SpinLattice2D::flip(int row, int col)
{
	(*this)(row,col) *= -1;
}

void SpinLattice2D::swap(int row1, int col1, int row2, int col2)
{
	(*this)(row1,col1) *= -1;
	(*this)(row2,col2) *= -1;
}

double SpinLattice2D::siteEnergy(int row, int col, double jConstant) const 
{
	double sum = 0;
	sum += (*this)(row,col) * (*this)(row+1,col);
	sum += (*this)(row,col) * (*this)(row-1,col);
	sum += (*this)(row,col) * (*this)(row,col+1);
	sum += (*this)(row,col) * (*this)(row,col-1);
	return -1.0 * sum * jConstant;
}

double SpinLattice2D::sitePairEnergy(int row1, int col1, int row2, int col2,double jConstant) const
{
	// Check to see if points are really the same, in which case just return the normal energy associated
	// with a single lattice site.
	if(row1==row2 && col1 == col2)
	{
		return siteEnergy(row1, col1, jConstant);
	}


	// Next check whether sites are nearest neighbours, in which case we need to account for over-counting.
	else if((*this).nearestNeighbours(row1, col1, row2, col2))
	{
		/* The only connecting that contributes to the over-counting is the connection that connects the sites 
		 * directly, therefore we are over or under-counting the energy by one unit. The amount by which we 
		 * over or under count is just the product of the spins and the jConstant parameter.
		 */
		double overCount = -1.0 * (*this)(row1,col1) * (*this)(row2,col2);
	 	return siteEnergy(row1, col1, jConstant)+siteEnergy(row2, col2, jConstant) - overCount;


	}

	// If sites are not the same, or nearest neighbours then they must be separated by at least one lattice site,
	// so there total energy is just the sum of their individual energies.
	else
	{
		return siteEnergy(row1, col1, jConstant)+siteEnergy(row2, col2, jConstant);
	}

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

	return -1.0 * jConstant * sum;
}

int SpinLattice2D::getRows() const
{
	return m_rowCount;
}

int SpinLattice2D::getCols() const
{
	return m_colCount;
}

bool SpinLattice2D::nearestNeighbours(int row1, int col1, int row2, int col2) const
{
	bool areNN = false;
	if((*this)(row1,col1)==(*this)(row2+1,col2)) { areNN = true;}
	if((*this)(row1,col1)==(*this)(row2-1,col2)) { areNN = true;}
	if((*this)(row1,col1)==(*this)(row2,col2+1)) { areNN = true;}
	if((*this)(row1,col1)==(*this)(row2,col2-1)) { areNN = true;}
	return areNN;

}

int SpinLattice2D::totalMag() const
{
	int sum = 0;
	for(const auto& mag : m_spinMatrix)
	{
		sum += mag;
	}
	return sum;
}