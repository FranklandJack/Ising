#ifndef SpinLattice2D_hpp
#define SpinLattice2D_hpp
#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

/**
 *\file 
 *\class SpinLattice2D
 *\brief Models a 2D spin lattice.
 *
 * A 2D spin lattice consists of a 2D array of ``spins'' which can either be ``up'' or ``'down'. 
 * This class currently does not support any 1-D implementation, i.e. declaring a SpinLattice2D 
 * with 1 column or 1 row will not give the expected results.
 */
class SpinLattice2D
{
public:
	/**
	 *\enum
	 *\brief Enumeration representing possible spins
	 */
	enum Spin
	{
		Up,
		Down,
		MAXSPINS,
	};
	/// static array that gives the values of the spins and acts as a map.
	static const int spinValues[MAXSPINS];
	private:
		/**
		 *\brief Member variable integer to represent the number of columns.
		 */
		int m_colCount;

		/**
		 *\brief Member variable integer to represent the number of rows.
		 */
		int m_rowCount;

		/**
		 *\brief Member variable array to represent the spins in the lattice. 
		 * 1D implementation for efficient memory use.
		 */
		std::vector<Spin> m_spinMatrix;

	public:

		/**
		 *\brief Creates a 2D spin lattice of specified dimensions; initially all spins up.
		 *\param rows integer representing desired number of rows in lattice.
		 *\param cols integer representing desired number of columns in lattice.
		 */
		SpinLattice2D(int rows, int cols);

		/**
		 *\brief Randomises all spins in array.
		 *\param generator reference to random engine to generate uniform random numbers on [0,1]
		 */
		void randomise(std::default_random_engine &generator);

		/**
		 *\brief Sets even proportions of each spin in blocks.
		 */
		void setEvenSpins();


		/**
		 *\brief Prints array as 2D matrix of +1 spin up and -1 spin down.
		 *\param out an output stream reference to stream to.
		 *\param spinLattice a const SpinLattice2D reference to be printed.
		 */
		friend std::ostream& operator<<(std::ostream &out, const SpinLattice2D &spinLattice);

		/** 
		 *\brief flips spin at specified position.
		 *\param row row of spin to be flipped.
		 *\param col column of spin to be flipped.
		 */
		void flip(int row, int col);

		/** 
		 *\brief swaps spin at specified positions.
		 *\param row1 row of first spin to be swapped.
		 *\param col1 column of first spin to be swapped.
		 *\param row2 row of second spin to be swapped.
		 *\param col2 column of second spin to be swapped.
		 */
		void swap(int row1, int col1, int row2, int col2);

		/**
		 *\brief Calculates energy associated with single lattice site.
		 *
		 * Energy is calculated according to the formula 
		 * E_site = - J * Sum_{nearest neighbours} S_site * S_neighbour,
		 * where S is the spin on a given site.
		 *
		 *\param row row of lattice site to calculate energy for.
		 *\param col column of lattice site to calculate energy for.
		 *\param jConstant constant floating point value representing the value of the J constant.
		 *\return floating point value representing the energy.
		 */
		double siteEnergy(int row, int col, const double jConstant) const;

		/**
		 *\brief Calculates energy associated with a pair lattice sites.
		 *
		 * Energy is calculated according to the formula 
		 * E_site = - J * Sum_{nearest neighbours} S_site * S_neighbour for each lattice site.
		 * This method takes into account the fact that selected sites may be nearest neighbours or  
		 * the same site and deals with those cases appropriately.
		 * 
		 *\param row1 row of first lattice site to calculate energy for.
		 *\param col1 column of first lattice site to calculate energy for.
		 *\param row2 row of second lattice site to calculate energy for.
		 *\param col2 column of second lattice site to calculate energy for.
		 *\param jConstant constant floating point value representing the value of the J constant.
		 *\return floating point value representing the energy.
		 */
		double sitePairEnergy(int row1, int col1,int row2,int col2, const double jConstant) const;

		/**
		 *\brief Calculates the total energy of the lattice.
		 *
		 * Energy is calculated according to the formula E = -J * Sum_{all nearest neighbours} S_site * S_neighbours,
		 * where S is the spin on a given site. 
		 *
		 *\param jConstant constant floating point value representing the value of the J constant.
		 */
		double latticeEnergy(const double jConstant) const;

		/**
		 *\brief Getter method for the number of columns in the spin lattice.
		 *\return integer value representing number of columns.
		 */
		int getCols() const;

		/**
		 *\brief Getter method for the number of rows in the spin lattice.
		 *\return integer value representing number of rows.
		 */
		int getRows() const;

		/**
		 *\brief Getter method for size of lattice = #columns * #rows
		 *\return integer value representing size of lattice.
		 */
		int getSize() const;

		/**
		 *\brief Calculates whether two sites are nearest neighbours.
		 *
		 * This method takes into account periodic boundary conditions.
		 *\param row1 integer value representing row index of first site.
		 *\param col1 integer value representing column index of first site.
		 *\param row2 integer value representing row index of second site.
		 *\param col2 integer value representing column index of second site.
		 *\return boolean value representing whether site are nearest neighbours or not.
		 */
		bool nearestNeighbours(int row1, int col1, int row2, int col2) const;

		/**
		 *\brief operator overload for getting the spin at a site.
		 *
		 * This method is implemented since the spins are stored internally as a 1D vector, hence 
		 * they need to be indexed in a special way in order to get the site that would correspond to 
		 * the (i,j) site in matrix notation. This function allows the caller to treat the lattice as a 
		 * 2D matrix without having to worry about the internal implementation.
		 *
		 *\param row row index of site.
		 *\param col column index of site.
		 *\return reference to spin stored at site so called can use it or set it.
		 */
		Spin& operator()(int row, int col);

		/** 
		 *\brief constant version of non-constant counterpart for use with constant SpinLattice2D object.
		 *
		 * See non-constant version for description.
		 *
		 *\param row row index of site.
		 *\param col column index of site.
		 *\return constant reference to spin stored at site so called can use it only.
		 */
		const Spin& operator()(int row, int col) const;

		/**
		 *\brief Calculates total magnetisation of the spin lattice.
		 *
		 * Uses the formula M = Sum_{all sites} S_{site} where S is the spin at a given site.
		 *\return integer value representing the total magnetisation.
		 */
		int totalMag() const;

};
#endif /* SpinLattice2D_hpp */