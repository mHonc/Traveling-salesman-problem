#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

class Simulated_annealing
{
public:
	double dlugosc_najlepszej_trasy;
	vector<int> najlepsza_trasa;

	Simulated_annealing(vector<vector<double>> _macierz_odleglosci, double _temperatura_poczatkowa, double _temperatura_chlodzenia, double _temperatura_minimalna, int _liczba_iteracji, bool _wyswietlanie = false);
	void znajdz_rozwiazanie();
	double dlugosc_trasy(vector<int> trasa);

private:
	vector<vector<double>> macierz_odleglosci;
	bool wyswietlanie;

	vector<int> obecna_trasa;
	double dlugosc_obecnej_trasy;

	double obecna_temperatura;
	double temperatura_chlodzenia;
	double temperatura_minimalna;
	int liczba_iteracji;

	void losowa_zamiana();
	double prawdopodobienstwo();
};