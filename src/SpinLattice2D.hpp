#ifndef SpinLattice2D_hpp
#define SpinLattice2D_hpp

#include <random>
#include <vector>
#include <iostream>
class SpinLattice2D
{
	private:
		int m_colCount;
		int m_rowCount;
		std::vector<int> m_spinMatrix;

		int& operator()(int,int);
		const int& operator()(int,int) const;

	public:
		SpinLattice2D(int,int);
		void randomise(std::default_random_engine);
		friend std::ostream& operator<<(std::ostream&, const SpinLattice2D&);
		void flip(int,int);
		void swap(int,int,int,int);
		double siteEnergy(int,int,double) const;
		double latticeEnergy(double) const;
		int getCols() const;
		int getRows() const;


};
#endif /* SpinLattice2D_hpp */