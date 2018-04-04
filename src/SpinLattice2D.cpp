#include "SpinLattice2D.hpp"
const int SpinLattice2D::spinValues[SpinLattice2D::MAXSPINS] = {+1,-1};

SpinLattice2D::SpinLattice2D(int rows, int cols): 	m_rowCount{rows},
													m_colCount{cols},
													m_spinMatrix(m_rowCount*m_colCount,SpinLattice2D::Up)
{
}

SpinLattice2D::Spin& SpinLattice2D::operator()(int row, int col)
{
	// Take into account periodic boundary conditions.
	row = (row + m_rowCount) % m_rowCount;
	col = (col + m_colCount) % m_colCount;

	// Return 1D index of 1D array corresponding to the 2D index.
	return m_spinMatrix[col + row * m_colCount];
}

const SpinLattice2D::Spin& SpinLattice2D::operator()(int row, int col) const
{
	// Take into account periodic boundary conditions we add extra m_rowCount and m_colCount
	// terms here to take into account the fact that the caller may be indexing with -1.
	row = (row + m_rowCount) % m_rowCount;
	col = (col + m_colCount) % m_colCount;

	// Return 1D index of 1D array corresponding to the 2D index.
	return m_spinMatrix[col + row * m_colCount];
}


void SpinLattice2D::randomise(std::default_random_engine &generator)
{
	// Create a ``uniform'' integer distribution for generating the spins. By using the MAXSPINS
	// value this distribution will automatically get updated if we add any more spins in the future.
	// Need to subtract 1 to account for fact range is inclusive and indexes start at 0.
	static std::uniform_int_distribution<int> distribution(0,static_cast<int>(SpinLattice2D::MAXSPINS)-1);
	for(auto& spin : m_spinMatrix)
	{
		spin = static_cast<SpinLattice2D::Spin>(distribution(generator));
	}
}

void SpinLattice2D::setEvenSpins()
{
	// Set first half of the spins up.
	for(int i = 0; i < m_spinMatrix.size()/2; ++i)
	{
		m_spinMatrix[i] = SpinLattice2D::Up;
	}

	// Set second half of spins down.
	for(int i = m_spinMatrix.size()/2; i < m_spinMatrix.size(); ++i)
	{
		m_spinMatrix[i] = SpinLattice2D::Down;
	}

}

std::ostream& operator<<(std::ostream& out, const SpinLattice2D& spinLattice)
{
	for(int row = 0; row < spinLattice.m_rowCount; ++row)
	{
		for(int col = 0; col < spinLattice.m_colCount; ++col)
		{
			out << std::showpos << SpinLattice2D::spinValues[spinLattice(row,col)] << ' ';
		}
		out << '\n';
	}
	return out;
}

void SpinLattice2D::flip(int row, int col)
{

	if((*this)(row, col) == SpinLattice2D::Up)
	{
		(*this)(row, col) = SpinLattice2D::Down;
	}
	else
	{
		(*this)(row, col) = SpinLattice2D::Up;
	}
}

void SpinLattice2D::swap(int row1, int col1, int row2, int col2)
{
	std::swap((*this)(row1,col1), (*this)(row2,col2));
}

double SpinLattice2D::siteEnergy(int row, int col, double jConstant) const
{
	// For a single site we just sum over the nearest neighbours
	// The operator() overload takes into account boundary conditions.
	double sum = 0;
	sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row+1, col)];
	sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row-1, col)];
	sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row, col+1)];
	sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row, col-1)];
	return -1.0 * jConstant * sum;
}

double SpinLattice2D::sitePairEnergy(int row1, int col1, int row2, int col2, const double jConstant) const
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
		/* The only connection that contributes to the over-counting is the connection that connects the sites
		 * directly, therefore we are over or under-counting the energy by one unit. The amount by which we
		 * over or under count is just the product of the spins and the jConstant parameter.
		 */
		double overCount = -1.0 * SpinLattice2D::spinValues[(*this)(row1,col1)] * SpinLattice2D::spinValues[(*this)(row2,col2)];
	 	return siteEnergy(row1, col1, jConstant) + siteEnergy(row2, col2, jConstant) - overCount;


	}

	// If sites are not the same, or nearest neighbours then they must be separated by at least one lattice site,
	// so there total energy is just the sum of their individual energies.
	else
	{
		return siteEnergy(row1, col1, jConstant) + siteEnergy(row2, col2, jConstant);
	}

}

double SpinLattice2D::latticeEnergy(double jConstant) const
{
	/* In order to calculate total energy without over counting we only consider the nearest neighbours
	 * below and to the right of each site, the periodic boundary conditions then insure that all sites
	 * are counted once.
	 */
	double sum = 0;
	for(unsigned row = 0; row < m_rowCount; ++row)
	{
		for(unsigned col = 0; col < m_colCount; ++col)
		{
			sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row, col+1)];
			sum += SpinLattice2D::spinValues[(*this)(row, col)] * SpinLattice2D::spinValues[(*this)(row+1, col)];
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

int SpinLattice2D::getSize() const
{
	return m_rowCount * m_colCount;
}

bool SpinLattice2D::nearestNeighbours(int row1, int col1, int row2, int col2) const
{
	// Need to check above/below and right/left including the periodic boundary conditions.

	// Check above and ``over the bottom'' of the lattice.
	if( (row1 == (row2+1)%m_rowCount) && (col1 == col2) ) { return true;}

	// Check below.
	if( (row2 == (row1+1)%m_rowCount) && (col1 == col2) ) { return true;}

	// Check the left.
	if( (col1 == (col2+1)%m_colCount) && (row1 == row2) ) { return true;}

	// Check to the right
	if( (col2 == (col1+1)%m_rowCount) && (row1 == row2) ) { return true;}

	// If it has reached this point there are no nearest neighbours.
	return false;

}

int SpinLattice2D::totalMag() const
{
	int sum = 0;
	for(const auto& mag : m_spinMatrix)
	{
		sum += SpinLattice2D::spinValues[mag];
	}
	return sum;
}
