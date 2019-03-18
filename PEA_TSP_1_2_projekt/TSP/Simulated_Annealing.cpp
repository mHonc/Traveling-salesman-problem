#include "Simulated_annealing.h"

using namespace std;

Simulated_annealing::Simulated_annealing(vector<vector<double>> _macierz_odleglosci, double _temperatura_poczatkowa, double _temperatura_chlodzenia, double _temperatura_minimalna, int _liczba_iteracji, bool _wyswietlanie)
{
	macierz_odleglosci = _macierz_odleglosci;
	wyswietlanie = _wyswietlanie;

	obecna_temperatura = _temperatura_poczatkowa;
	temperatura_chlodzenia = _temperatura_chlodzenia;
	temperatura_minimalna = _temperatura_minimalna;
	liczba_iteracji = _liczba_iteracji;

	//poczatkowa trasa (1 - 2 - ... N)
	for (unsigned int i = 1; i < macierz_odleglosci.size(); i++)
		obecna_trasa.push_back(i);
	dlugosc_obecnej_trasy = dlugosc_trasy(obecna_trasa);

	najlepsza_trasa = obecna_trasa;
	dlugosc_najlepszej_trasy = dlugosc_obecnej_trasy;

}

void Simulated_annealing::znajdz_rozwiazanie()
{
	while (obecna_temperatura > temperatura_minimalna)
	{
		for (int j = 0; j < liczba_iteracji; j++)
		{
			losowa_zamiana();
			dlugosc_obecnej_trasy = dlugosc_trasy(obecna_trasa);
			
			if (dlugosc_obecnej_trasy < dlugosc_najlepszej_trasy
				//pozwalamy na zmiane na gorsze z pewnym prawdopodobienstwem w nadzieii na uzyskanie lepszego rozwiazania
				//w nowym sasiedztwie, ma to na celu unikniecie minimum lokalnego, wraz z kolejnymi iteracjami
				//ponizsze kryterium zostaje spelnione coraz rzadziej
				|| ((double)rand() / (double)RAND_MAX) < prawdopodobienstwo())
			{
				najlepsza_trasa = obecna_trasa;
				dlugosc_najlepszej_trasy = dlugosc_obecnej_trasy;
			}

			if (wyswietlanie) // wyswietlanie wszystkich znalezionych sciezek 
			{
				cout << "Testowana trasa: 0, ";
				for (unsigned int i = 0; i < obecna_trasa.size(); i++)
					std::cout << obecna_trasa[i] << ", ";
				cout << "0" << std::endl;
				cout << "Jej dlugosc: " << dlugosc_obecnej_trasy << std::endl;
				cout << std::endl;
			}
		}
		obecna_temperatura *= temperatura_chlodzenia;
	}
}

void Simulated_annealing::losowa_zamiana()
{
	//zamieniamy dwa losowe miasta w otoczeniu
	int ilosc_miast = najlepsza_trasa.size();
	int a = std::rand() % ilosc_miast;
	int b = std::rand() % ilosc_miast;
	while (a == b) b = std::rand() % ilosc_miast;

	obecna_trasa = najlepsza_trasa;
	int tmp = obecna_trasa[a];
	obecna_trasa[a] = obecna_trasa[b];
	obecna_trasa[b] = tmp;
}

double Simulated_annealing::prawdopodobienstwo()
{
	double potega = -((dlugosc_obecnej_trasy - dlugosc_najlepszej_trasy) / obecna_temperatura);
	return pow(M_E, potega);
}

double Simulated_annealing::dlugosc_trasy(std::vector<int> trasa)
{
	//zaczynamy od miasta 0 i zliczamy koszt przejscia trasy po kolejnych miastach
	double dlugosc = 0;
	int poprzedni_element = 0;
	for (unsigned int i = 0; i < trasa.size(); i++)
	{
		dlugosc += macierz_odleglosci[poprzedni_element][trasa[i]];
		poprzedni_element = trasa[i];
	}
	dlugosc += macierz_odleglosci[poprzedni_element][0];
	return dlugosc;
}

