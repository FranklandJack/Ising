#include "SpinBitLattice2D.hpp"

SpinBitLattice2D::SpinBitLattice2D(int rows, int cols): m_rowCount(rows), m_colCount(cols), m_spinMatrix(rows*cols,1)
{
}

boost::dynamic_bitset<>::reference SpinBitLattice2D::operator()(int row, int col)
{
	row %= m_rowCount;
	col %= m_colCount;
	return m_spinMatrix[col + row * m_colCount];
}

const boost::dynamic_bitset<>::const_reference SpinBitLattice2D::operator()(int row, int col) const
{
	row %= m_rowCount;
	col %= m_colCount;
	return m_spinMatrix[col + row * m_colCount];
}


void SpinBitLattice2D::randomise(std::default_random_engine generator)
{
	std::bernoulli_distribution distribution(0.5);
	for(int spin = 0; spin < m_spinMatrix.size();++spin)
	{
		if(distribution(generator)) m_spinMatrix[spin].flip();
	}
}

std::ostream& operator<<(std::ostream& out, const SpinBitLattice2D& spinLattice)
{
	for(int row = 0; row < spinLattice.m_rowCount; ++row)
	{
		for(int col = 0; col < spinLattice.m_colCount; ++col)
		{
			out << static_cast<bool>(spinLattice(row,col)) << ' ';
		}

		out << '\n';
	}

	return out;
}

void SpinBitLattice2D::flip(int row,int col)
{
	(*this)(row,col).flip();
}

void SpinBitLattice2D::swap(int row1,int col1,int row2,int col2)
{
	int temp;
	temp = (*this)(row1,col1);
	(*this)(row1,col1) = (*this)(row2,col2);
	(*this)(row2,col2) = temp;
}

//double SpinBitLattice2D::siteEnergy(int row, int col, double jConsant) const


//double SpinBitLattice2D::sitePairEnergy(int,int,int,int,double) const;
//double SpinBitLattice2D::latticeEnergy(double) const;

int SpinBitLattice2D::getCols() const
{
	return m_colCount;
}

int SpinBitLattice2D::getRows() const
{
	return m_rowCount;
}
//bool SpinBitLattice2D::nearestNeighbours(int,int,int,int) const;

int SpinBitLattice2D::totalMag() const
{
	double sum = 0;
	for(int site = 0; site < m_spinMatrix.size(); ++site)
	{
		sum += (static_cast<bool>(m_spinMatrix[site]) ? 1 : -1);
	}
	return sum;
}