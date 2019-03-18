#include "pch.h"
#include "BF.h"


BF::BF()
{
}


BF::~BF()
{
	delete[] Ha;
	delete[] Ha_temp;
	delete[] visited;

	for (int i = 0; i < n; i++)
	{
		delete[] A[i];
		delete[] W[i];
	}

	delete[] A;
	delete[] W;
}

bool BF::build_from_file()
{
	ifstream plik;
	plik.open("dane.txt", std::ios::in | std::ios::out);
	if (!plik.good())
		return false;

	plik >> n;                 // Czytamy liczbê wierzcho³ków
	m = (n * (n - 1)) / 2;

	// Tworzymy struktury dynamiczne i inicjujemy je

	Ha = new int[n];
	Ha_temp = new int[n];
	visited = new bool[n];
	A = new bool *[n];
	W = new int *[n];
	for (int i = 0; i < n; i++)
	{
		A[i] = new bool[n];
		W[i] = new int[n];
		visited[i] = false;
		for (int j = 0; j < n; j++)
		{
			A[i][j] = false;
			W[i][j] = 0;
		}
	}
	q = qtemp = 0;

	// Odczytujemy dane wejœciowe


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A[i][j] = true;     // KrawêdŸ x-y
			plik >> W[i][j];  // wczytanie wagi krawêdzi x-y
			cout.width(3);
			cout << W[i][j] << " ";
		}
		cout << endl;
	}

	plik.close();
	cout << endl;
	return true;
}

void BF::TSP()
{
	d = MAXINT;
	dh = v0 = 0;
	Brute_force(v0);
	if (q)                       // jesli znaleziono cykl hamiltona
	{
		for (int i = 0; i < q; i++) 
			cout << Ha[i] << " - ";
		cout << v0 << endl;
		cout << "koszt = " << d << endl;
	}
	else 
		cout << "Brak cyklu hamiltona!" << endl;

	cout << endl;
}

void BF::Brute_force(int v)
{
	int u;
	Ha_temp[qtemp++] = v;                // zapamiêtujemy bie¿¹cy wierzcho³ek na liœcie pomocniczej

	if (qtemp < n)                   // jeœli brak œcie¿ki Hamiltona, to jej szukamy
	{
		visited[v] = true;            // Oznaczamy bie¿¹cy wierzcho³ek jako odwiedzony
		for (u = 0; u < n; u++)        // Przegl¹damy s¹siadów wierzcho³ka v
			if (A[v][u] && !visited[u])  // Szukamy nieodwiedzonego jeszcze s¹siada
			{
				dh += W[v][u];            // Dodajemy wagê krawêdzi v-u do sumy
				Brute_force(u);           // Rekurencyjnie wywo³ujemy szukanie cyklu Hamiltona
				dh -= W[v][u];            // Usuwamy wagê krawêdzi z sumy
			}
		visited[v] = false;           // Zwalniamy bie¿¹cy wierzcho³ek
	}
	else if (A[v0][v])               // Jeœli znaleziona œcie¿ka jest cyklem Hamiltona
	{
		dh += W[v][v0];               // to sprawdzamy, czy ma najmniejsz¹ sumê wag
		if (dh < d)
		{
			d = dh;                     // zapamiêtujemy tê sumê
			for (int u = 0; u < qtemp; u++)  // oraz kopiujemy liste qtemp do q
				Ha[u] = Ha_temp[u];
			q = qtemp;
		}
		dh -= W[v][v0];               // Usuwamy wagê krawêdzi v-v0 z sumy
	}
	qtemp--;                        // Usuwamy bie¿¹cy wierzcho³ek ze œcie¿ki
}