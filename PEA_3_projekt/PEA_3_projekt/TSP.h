#pragma once


class TSP
{
public:
	TSP();
	~TSP();

	int** distances;
	int dimension;
	int getObjectiveFunctionValue(int *solution, int solutionLength);
	bool loadTSP();
};


