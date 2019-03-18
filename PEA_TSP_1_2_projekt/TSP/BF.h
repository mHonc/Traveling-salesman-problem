#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class BF
{
	const int MAXINT = 999999999;
	int n, m, v0, d, dh, q, qtemp; //liczba wierzcholkow, liczba krawedzi, wierzcholek poczatkowy
	                               //suma wag krawedzi cyklu hamiltona, pomocnicza suma wag, ilosc odwiedzonych wierzcholkow,
	                               //pomocnicza ilosc odwiedzonych wierzcholkow
	bool **A;                      // Macierz s¹siedztwa
	int **W;                       // Macierz wag krawêdzi
	int *Ha, *Ha_temp;             // Stosy w tablicy
	bool *visited;                 // Tablica odwiedzin


public:
	BF();
	~BF();
	bool build_from_file();
	void TSP();
	void Brute_force(int v);

};