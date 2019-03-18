#pragma once
#include <fstream>
#include <iostream>
using namespace std;

class Dynamic
{
	int n;
	const int INFTY = 999999999;
	int **dp; //tablica pamietajaca dlugosc najkrotszej sciezki maski konczacej sie w wierzcholku i (dp[mask][i])
	int **t; //tablica wag
public:
	Dynamic();
	~Dynamic();
	bool build_from_file();
	void TSP();
	template <class T> const T& min(const T& a, const T& b);
};

template<class T>
inline const T & Dynamic::min(const T & a, const T & b)
{
	return !(b < a) ? a : b;
}
