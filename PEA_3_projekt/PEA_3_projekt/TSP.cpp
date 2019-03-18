#include "pch.h"
#include "TSP.h"
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

TSP::TSP()
{
}


TSP::~TSP()
{
	if (distances != NULL) {
		for (int i = 0; i < dimension; i++) {
			delete[] distances[i];
		}
		delete[] distances;
	}
}

//zwrocenie kosztu sciezki
int TSP::getObjectiveFunctionValue(int * solution, int solutionLength)
{
	int cost = 0;
	for (int i = 0; i < solutionLength - 1; i++) {
		cost += distances[solution[i]][solution[i + 1]];
	}
	return cost;
}

bool TSP::loadTSP()
{
		// wczytanie danych z pliku i inicjalizacja potrzebnych nam zmiennych
		fstream file;
		file.open("dane.txt", ios::in);
		if (!file.good()) {
			cout << "\nNie udalo sie otworzyc pliku!\n";
			file.close();
			file.clear();
			return false;
		}
		else {

			file >> dimension;
			cout << dimension << endl;

			distances = new int*[dimension];
			for (int i = 0; i < dimension; i++) {
				distances[i] = new int[dimension];
			}

			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < dimension; j++)
				{
					file >> distances[i][j];
					cout << distances[i][j] << "  ";
				}
				cout << endl;
			}

			file.close();
			file.clear();
			return true;
	}
}
