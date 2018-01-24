#ifndef SpinBitLattice2D_hpp
#define SpinBitLattice2D_hpp
#include <vector>
#include <random>
#include <iostream>
class SpinBitLattice2D
{
	private:
		int m_colCount;
		int m_rowCount;
		std::vector<int> m_spinMatrix;
		
	public:
		SpinBitLattice2D(int,int);
		void randomise(std::default_random_engine);
		friend std::ostream& operator<<(std::ostream&, const SpinBitLattice2D&);
		void flip(int,int);
		void swap(int,int,int,int);
		double siteEnergy(int,int,double) const;
		double sitePairEnergy(int,int,int,int,double) const;
		double latticeEnergy(double) const;
		int getCols() const;
		int getRows() const;
		bool nearestNeighbours(int,int,int,int) const;
		int& operator()(int,int);
		const int& operator()(int,int) const;
		int totalMag() const;
};
#endif /* SpinBitLattice2D */