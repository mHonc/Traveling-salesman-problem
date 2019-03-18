#include "Dynamic.h"


Dynamic::Dynamic()
{
}


Dynamic::~Dynamic()
{
	for (int i = 0; i < n; i++)
	{
		delete[] t[i];
		delete[] dp[i];
	}

	delete[] t;
	delete[] dp;
}

bool Dynamic::build_from_file()
{
	ifstream plik;
	plik.open("dane.txt", std::ios::in | std::ios::out);
	if (!plik.good())
		return false;

	plik >> n;                 // Czytamy liczbê wierzcho³ków

	// Tworzymy struktury dynamiczne i inicjujemy je
	t = new int *[n];
	for (int i = 0; i < n; i++)
	{

		t[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			t[i][j] = 0;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			plik >> t[i][j];  // wczytanie wagi krawêdzi x-y
			cout.width(3);
			cout << t[i][j] << " ";
		}
		cout << endl;
	}

	dp = new int *[1 << n];   // 2^n mozliwych masek
	for (int i = 0; i < (1 << n); i++)
	{
		dp[i] = new int[n];
	}

	
	plik.close();
	cout << endl;
	return true;
}

void Dynamic::TSP()
{
	for (int i = 0; i < n; ++i)
		dp[0][i] = dp[1 << 0][i] = INFTY;
	dp[1 << 0][0] = 0;
	for (int mask = 2; mask < (1 << n); ++mask)
		for (int i = 0; i < n; ++i) //podzial na podzbiory
		{
			dp[mask][i] = INFTY;
			if (mask & (1 << i)) // AND maski i przesuniecia bitowego 1 o i bitow w prawo, sprawdzenie czy bit znajdujacy sie na 
								 //odpowiedniej pozycji jest zapalony, czy nie
			{
				int mask1 = mask ^ (1 << i); //XOR maski i przesuniecia bitowego, zapala i-ty bit w masce
				for (int j = 0; j < n; ++j) //obliczenie wartosci dla wszystkich podzbiorow znajdujacych sie w rozpatrywanym podzbiorze
					if (mask1 & (1 << j)) //AND, sprawdzenie czy bit znajdujacy sie na odpowiedniej pozycji jest zapalony, czy nie
						dp[mask][i] = min(dp[mask][i], dp[mask1][j] + t[j][i]); //minimum danej sciezki
			}
		}
	int wynik = INFTY;
	for (int i = 0; i < n; ++i)
		wynik = min(wynik, dp[(1 << n) - 1][i] + t[i][0]); //rozpatrzenie przejscia przez ostatnia krawedz i wybranie najmniejszej mozliwej wartosci

	cout << wynik << endl;
}


