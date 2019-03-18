#include "pch.h"
#include "Genetic.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <math.h>
#include <ctime>
using namespace std;

Genetic::Genetic()
{
}


Genetic::~Genetic()
{
}

//zamiana dwoch miast
int * Genetic::swap(int city1, int city2, int * solution)
{
	int temp = solution[city1];
	solution[city1] = solution[city2];
	solution[city2] = temp;
	return solution;
}

void Genetic::printSolution(int * solution, int solutionLength)
{
	for (int i = 0; i < solutionLength - 1; i++) {
		cout << solution[i] << "->";
	}
	cout << solution[solutionLength - 1];
	cout << "\n";
}

// Algorytm pozwalaj¹cy na pseudolosowe poszukiwanie
// rozwi¹zañ w zale¿noœci od rozmiaru turnieju.Rozmiar turnieju decyduje o tym,
// czy selekcja jest w pe³ni(pseudo)losowa, czy te¿ faworyzowane s¹ osobniki
// o najlepszych cechach oraz jak bardzo s¹ faworyzowane.
// Dla wartosci tournamentSize 1 selekcja losowa, wraz ze wzrostem parametru tournamentSize rosnie prawdopodobienstwo wyboru lepszego osobnika
int * Genetic::tournamentSelection(int ** P, int tournamentSize, int solutionLength, int popsize, int * fitness, int * best, int * next)
{

	int r1, r2;

	r1 = rand() % (popsize);			// losowanie besta
	for (int i = 0; i < solutionLength; i++) {
		best[i] = P[r1][i];
	}

	for (int i = 1; i < tournamentSize; i++) {

		r2 = rand() % (popsize);		// losowanie nexta
		for (int j = 0; j < solutionLength; j++) {
			next[j] = P[r2][j];
		}

		if (fitness[r1] > fitness[r2]) {
			for (int k = 0; k < solutionLength; k++) {
				best[k] = next[k];
			}
		}
	}
	return best;
}

int * Genetic::crossoverOX(int * parentA, int * parentB, int * children, int solutionLength)
{
	int r1, r2, saveIndex, count = 0;
	boolean ifGood;
	r1 = rand() % (solutionLength - 2);
	do {
		r2 = rand() % (solutionLength - 2);
	} while (r1 == r2);

	if (r1 > r2) {
		// miedzy wycieciami r1, r2: przepisanie parentA, inaczej przekopiowanie srodka
		for (int i = 0; i < (r1 - r2); i++) {
			children[r2 + 1 + i] = parentA[r2 + 1 + i];
		}

		// dopisywanie reszty po kolei wg algorytmu
		for (int i = 0; count < (solutionLength - (r1 - r2) - 1); i++) {
			ifGood = true;
			for (int j = 0; j < (r1 - r2); j++) {
				if ((r1 + 1 + i) > (solutionLength - 2)) {
					saveIndex = r1 + 2 + i - solutionLength;
					if (parentB[r1 + 2 + i - solutionLength] == children[r2 + 1 + j]) { // sprawdzamy czy elementu nie ma w zbiorze
						ifGood = false;
						break;
					}
				}
				else {
					saveIndex = r1 + 1 + i;
					if (parentB[r1 + 1 + i] == children[r2 + 1 + j]) { // sprawdzamy czy elementu nie ma w zbiorze
						ifGood = false;
						break;
					}
				}
			}
			if (ifGood) { //jesli nie ma elementu to go dopisujemy
				if ((r1 + 1 + count) > (solutionLength - 2)) {
					children[r1 + 2 + count - solutionLength] = parentB[saveIndex];
				}
				else {
					children[r1 + 1 + count] = parentB[saveIndex];
				}
				count++;
			}
		}
		children[solutionLength - 1] = children[0];
	}
	else {	// r2 > r1
			// miedzy wycieciami r1, r2: przepisanie parentA
		for (int i = 0; i < (r2 - r1); i++) {
			children[r1 + 1 + i] = parentA[r1 + 1 + i];
		}

		// dopisywanie reszty po kolei wg algorytmu
		for (int i = 0; count < (solutionLength - (r2 - r1) - 1); i++) {
			ifGood = true;
			for (int j = 0; j < (r2 - r1); j++) {
				if ((r2 + 1 + i) > (solutionLength - 2)) {
					saveIndex = r2 + 2 + i - solutionLength;
					if (parentB[r2 + 2 + i - solutionLength] == children[r1 + 1 + j]) {
						ifGood = false;
						break;
					}
				}
				else {
					saveIndex = r2 + 1 + i;
					if (parentB[r2 + 1 + i] == children[r1 + 1 + j]) {
						ifGood = false;
						break;
					}
				}
			}
			if (ifGood) {
				if ((r2 + 1 + count) > (solutionLength - 2)) {
					children[r2 + 2 + count - solutionLength] = parentB[saveIndex];
				}
				else {
					children[r2 + 1 + count] = parentB[saveIndex];
				}
				count++;
			}
		}
		children[solutionLength - 1] = children[0];
	}
	return children;
}

int * Genetic::mutateSwap(int * children, int solutionLength)
{
	int r1, r2;
	r1 = rand() % (solutionLength - 1);
	do {
		r2 = rand() % (solutionLength - 1);
	} while (r1 == r2);
	children = swap(r1, r2, children);

	children[solutionLength - 1] = children[0];

	return children;
}

int * Genetic::mutateInvert(int *children, int solutionLength) {
	int r1, r2;
	r1 = rand() % (solutionLength - 1);
	do {
		r2 = rand() % (solutionLength - 1);
	} while (r1 == r2);

	//children = swap(r1, r2, children);
	if (r2 > r1) {
		for (int i = 0; i < ((r2 - r1) / 2); i++) {
			children = swap(r1, r2, children);
			r1++;
			r2--;
		}
	}
	else {
		for (int i = 0; i < ((r1 - r2) / 2); i++) {
			children = swap(r1, r2, children);
			r1--;
			r2++;
		}
	}


	children[solutionLength - 1] = children[0];

	return children;
}
