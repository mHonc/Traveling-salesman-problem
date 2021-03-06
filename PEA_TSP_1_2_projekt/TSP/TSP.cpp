// TSP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Windows.h"
#include "Simulated_annealing.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


template<typename TimeT = std::chrono::microseconds,
	typename ClockT = std::chrono::high_resolution_clock,
	typename DurationT = double>//pomiar czasu w nanosekundach
	class Stopwatch
{
private:
	std::chrono::time_point<ClockT> _start, _end;
public:
	Stopwatch()
	{
		start();
	}
	void start()
	{
		_start = _end = ClockT::now();
	}
	DurationT stop()
	{
		_end = ClockT::now();
		return elapsed();
	}
	DurationT elapsed()
	{
		auto delta = std::chrono::duration_cast<TimeT>(_end - _start);
		return delta.count();
	}
};


void symulowane_wyzarzanie();
std::vector<std::vector<double>> wczytaj_macierz_odleglosci();


int main()
{
	symulowane_wyzarzanie();

	system("pause");
	return 0;
}


void symulowane_wyzarzanie()
{

	std::vector<std::vector<double>> macierz_odleglosci;
	macierz_odleglosci = wczytaj_macierz_odleglosci();
	std::cout << "Ilosc wczytanych miast: " << macierz_odleglosci.size() << std::endl;
	std::cout << std::endl;

	if (macierz_odleglosci.size() < 2) return;

	double temperatura_poczatkowa = 10000.0;
	double temperatura_chlodzenia = 0.9999;
	double temperatura_minimalna = 0.0001;
	int liczba_iteracji = (int)pow((double)macierz_odleglosci.size(), (double)2) / 4; // n^2/4 wydaje sie byc optymalnym parametrem

	std::cout << "Parametry:" << std::endl;
	std::cout << "Temperatura poczatkowa - " << temperatura_poczatkowa << std::endl;
	std::cout << "Temperatura chlodzenia - " << temperatura_chlodzenia << std::endl;
	std::cout << "Temperatura minimalna - " << temperatura_minimalna << std::endl;
	std::cout << "Liczba iteracji dla poszczegolnych temperatur - n^2/4=" << liczba_iteracji << std::endl; 

	Simulated_annealing TSP = Simulated_annealing(macierz_odleglosci, temperatura_poczatkowa, temperatura_chlodzenia, temperatura_minimalna, liczba_iteracji, false);
	
	Stopwatch<> sw; //pomiar czasu
	TSP.znajdz_rozwiazanie();
	sw.stop();
	cout << "czas: " << sw.elapsed() << " mikrosekund\n";


	std::cout << "Najlepsza trasa: 0 - ";
	for (unsigned int i = 0; i < TSP.najlepsza_trasa.size(); i++)
		std::cout << TSP.najlepsza_trasa[i] << " - ";
	std::cout << "0" << std::endl;
	std::cout << "Jej dlugosc: " << TSP.dlugosc_najlepszej_trasy << std::endl;
	std::cout << std::endl;
}


std::vector<std::vector<double>> wczytaj_macierz_odleglosci()
{
	std::vector<std::vector<double>> macierz_odleglosci;

	std::ifstream plik;
	plik.open("dane.txt");

	if (plik.is_open())
	{
		int liczba_miast;
		int odleglosc;


		plik >> liczba_miast;

		for (int i = 0; i < liczba_miast; i++)
		{
			std::vector<double> wiersz_macierzy_odleglosci;
			macierz_odleglosci.push_back(wiersz_macierzy_odleglosci);

			for (int j = 0; j < liczba_miast; j++)
			{
				plik >> odleglosc;
				macierz_odleglosci[i].push_back(odleglosc);
				cout.width(3);
				cout << macierz_odleglosci[i][j] << " ";
			}
			cout << "\n\n";
		}
		plik.close();
	}
	return macierz_odleglosci;
}


	//int n;
	//cout << "[1]BF\n" << "[2]Dynamiczne\n\n";
	//cin >> n;

	//switch (n)
	//{
	//case 1:
	//{
	//	BF* bf = new BF();
	//	bf->build_from_file();

	//	Stopwatch<> sw; //pomiar czasu
	//	bf->TSP();
	//	sw.stop();
	//	cout << "czas: " << sw.elapsed() << " mikrosekund\n";

	//	delete bf;
	//	break;
	//}
	//case 2:
	//{
	//	Dynamic* dn = new Dynamic();
	//	dn->build_from_file();

	//	Stopwatch<> sw;
	//	dn->TSP();
	//	sw.stop();
	//	cout << "czas: " << sw.elapsed() << " mikrosekund\n";

	//	delete dn;
	//	break;
	//}
	//}
	//system("pause");
	
	//return 0;

