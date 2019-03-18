#pragma once
class Genetic
{
public:
	Genetic();
	~Genetic();


	int* swap(int city1, int city2, int* solution);

	void printSolution(int* solution, int solutionLength);

	int* tournamentSelection(int **P, int tournamentSize, int solutionLength, int popsize, int *fitness, int *best, int *next);

	int* crossoverOX(int *parentA, int *parentB, int* children, int solutionLength);

	int* mutateSwap(int *children, int solutionLength);

	int * mutateInvert(int * children, int solutionLength);
};

