/* Program wykorzystujacy algorytm genetyczny do rozwiazania problemu TSP dla biblioteki TSPLIB
http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/ */

#include "pch.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <math.h>
#include <ctime>
#include "TSP.h"
#include "Genetic.h"

using namespace std;

long long int read_QPC()    // do pomiaru czasu
{
	LARGE_INTEGER count;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&count);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return((long long int)count.QuadPart);
}



int main() 
{
	// parametry algorytmu
	int numberOfIterations = 5000;      // liczba iteracji do zakonczenia poszukiwan
	int popsize = 2000;			       // wielkosc populacji
	int t = 50;					       // tournamentSize - dla selekcji turniejowej, dla wartosci 1 selekcja losowa, 
	                                      //zas dla wartosci >2 zwiekszanie wartosci parametru
	double mutProb = 0.8;			   // prawdopodobienstwo mutacji. Proponowane wartosci: <0.65-0,85>
	double crossProb = 0.1;		       // prawdopodobienstwo krzyzowki. Proponowane wartosci: <0.01-0,1>
	srand(time(NULL));

	TSP *tsp = new TSP();
			
	crossProb *= 100;
	mutProb *= 100;

	long long int frequency, start, elapsed;
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

	if (tsp->loadTSP())
	{
		double avgTime = 0;
		double avgCost = 0;
		int minCost = (std::numeric_limits<int>::max)(); //ustawienie kosztu na maksymalny
		//for (int m = 0; m < 5; m++) {
		Genetic *genetic = new Genetic();
		int solutionLength = tsp->dimension + 1;
		int r1, r2;
		double r3;

		int bestCost = (std::numeric_limits<int>::max)();

		int *childrenA = new int[solutionLength];
		int *childrenB = new int[solutionLength];
		int* fitness = new int[popsize];
		int* parentA = new int[solutionLength];
		int* parentB = new int[solutionLength];
		int* next = new int[solutionLength];
		int* bestSol = new int[solutionLength];

		int** P = new int*[popsize];
		for (int i = 0; i < popsize; i++) {
			P[i] = new int[solutionLength];
		}

		int** Q = new int*[popsize];	// nowa generacja (pokolenie, populacja)
		for (int i = 0; i < popsize; i++) {
			Q[i] = new int[solutionLength];
		}

		start = read_QPC();	// rozpoczecie pomiaru czasu algorytmu

		for (int i = 0; i < popsize; i++) {	// losowanie populacji rodzicow
			for (int j = 0; j < solutionLength - 1; j++) {
				P[i][j] = j;
			}

			for (int j = 0; j < solutionLength - 1; j++) {
				r1 = rand() % (solutionLength - 1);
				r2 = rand() % (solutionLength - 1);
				P[i] = genetic->swap(r1, r2, P[i]);
			}
			P[i][solutionLength - 1] = P[i][0];
		}

		for (int i = 0; i < solutionLength; i++) {	// pierwsze rozwiazanie ustawione poczatkowo
			bestSol[i] = P[0][i];					// jako najlepsze
		}


		for (int i = 0; i < numberOfIterations; i++) { // glowna petla algorytmu
			
			
			// zapamiętanie najlepszego dotychczasowego rozwiązania. Jakość
			// osobników będzie miała znaczenie dla selekcji rodziców
			for (int j = 0; j < popsize; j++) {
				fitness[j] = tsp->getObjectiveFunctionValue(P[j], solutionLength);	// accessFitness

				if (fitness[j] < bestCost) {
					for (int k = 0; k < solutionLength; k++) {
						bestSol[k] = P[j][k];
					}
					bestCost = fitness[j];
				}
			}

			for (int j = 0; j < popsize; j += 2) {
				parentA = genetic->tournamentSelection(P, t, solutionLength, popsize, fitness, parentA, next);
				parentB = genetic->tournamentSelection(P, t, solutionLength, popsize, fitness, parentB, next);

				r3 = rand() % 100;
				if (r3 < crossProb)
					childrenA = genetic->crossoverOX(parentA, parentB, childrenA, solutionLength);
				else {
					for (int u = 0; u < solutionLength; u++) {
						childrenA[u] = parentA[u];
					}
				}
				r3 = rand() % 100;
				if (r3 < crossProb)
					childrenB = genetic->crossoverOX(parentB, parentA, childrenB, solutionLength);
				else {
					for (int u = 0; u < solutionLength; u++) {
						childrenB[u] = parentB[u];
					}
				}

				r3 = rand() % 100;
				if (r3 < mutProb)
					childrenA = genetic->mutateInvert(childrenA, solutionLength);
				r3 = rand() % 100;
				if (r3 < mutProb)
					childrenB = genetic->mutateInvert(childrenB, solutionLength);

				for (int k = 0; k < solutionLength; k++) {	// Q<-childrens
					Q[j][k] = childrenA[k];
					Q[j + 1][k] = childrenB[k];
				}
			}

			for (int j = 0; j < popsize; j++) {			// P<-Q
				for (int k = 0; k < solutionLength; k++) {
					P[j][k] = Q[j][k];
				}
			}
		}

		elapsed = read_QPC() - start;	// koniec pomiar czasu po zakonczeniu petli
		cout << "Wyniki znaleziono w: "
			<< setprecision(0) << (1000000.0 * elapsed) / frequency << " us"
			<< "\nDlugosc najkrotszej znalezionej trasy:  " << bestCost << "\nSciezka: ";
		genetic->printSolution(bestSol, solutionLength);

		avgTime += elapsed;
		avgCost += bestCost;
		//if (bestCost < minCost) minCost = bestCost;

		for (int i = 0; i < popsize; i++) 
		{
			delete[] P[i];
		}
		delete[] P;

		for (int i = 0; i < popsize; i++) 
		{
			delete[] Q[i];
		}
		delete[] Q;

		delete[] bestSol;
		delete[] fitness;
		delete[] parentA;
		delete[] parentB;
		delete[] next;
		delete[] childrenA;
		delete[] childrenB;
		delete genetic;
		/*}
		avgTime /= 5;
		avgCost /= 5;
		cout << "==================================================";
		cout << "\nWyniki znaleziono srednio w czasie: "
			<< setprecision(0) << (1000.0 * avgTime) / frequency << " ms"
			<< "\nSrednia dlugosc znalezionych tras:  " << avgCost
			<< "\nDlugosc najkrotszej znalezionej trasy:  " << minCost << "\n";*/

	}
		
	system("pause");
	delete tsp;

	return 0;
}