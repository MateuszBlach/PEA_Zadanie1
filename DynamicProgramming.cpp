#pragma once
#include "DynamicProgramming.h"
#include "MyFunctions.h"
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <functional>

using namespace std;
using namespace std::chrono;

DynamicProgramming::DynamicProgramming() {
	numberOfCities = 0;
	matrix = NULL;
	minPathLength = INT_MAX;
	dp = NULL;
	parent = NULL;
}

DynamicProgramming::~DynamicProgramming() {
	if (numberOfCities != 0) {
		for (int i = 0; i < numberOfCities; i++) {
			delete[] matrix[i];
			delete[] dp[i];
			delete[] parent[i];
		} 
		delete[] matrix;
		delete[] dp;
		delete[] parent;
		numberOfCities = 0;
	}
}

int DynamicProgramming::manageDynamicProgrammingMenu() {
	int choice, numberOfV, currCity, mask;
	string filePath;

	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::duration duration;
	long time;

	cin >> choice;
	switch (choice) {
	case 1:
		system("CLS");
		std::cout << "Podaj sciezke do pliku: ";
		cin >> filePath;
		loadFromFile(filePath);
		display();
		break;
	case 2:
		system("CLS");
		std::cout << "Podaj ilosc wierzcholkow: ";
		cin >> numberOfV;
		generateRandom(numberOfV);
		display();
		break;
	case 3:
		system("CLS");
		display();
		break;
	case 4:
		system("CLS");
		start = steady_clock::now();
		solveTSP();
		duration = steady_clock::now() - start;
		time = duration_cast<microseconds>(duration).count();
		cout << "Czas wykonania: " << time << "microseconds" << endl;

		cout << "Najlepsza trasa: ";
		currCity = 0;
		mask = 1;
		for (int i = 0; i < numberOfCities - 1; i++) {
			cout << currCity << "->";
			currCity = parent[currCity][mask];
			mask |= (1 << currCity);
		}
		cout << currCity << "->0" << endl;

		cout << endl;
		cout << "Dlugosc trasy: " << minPathLength << endl;
		break;
	default:
		DynamicProgramming::~DynamicProgramming();
		break;
	}
	return choice;

}

void DynamicProgramming::display() {
	cout << "   ";
	for (int i = 0; i < numberOfCities; i++) cout << setw(4) << i;
	cout << endl;
	for (int i = 0; i < numberOfCities; i++)
	{
		cout << setw(3) << i;
		for (int j = 0; j < numberOfCities; j++) cout << setw(4) << matrix[i][j];
		cout << endl;
	}
}

void DynamicProgramming::loadFromFile(string path) {
	ifstream  myFile(path);
	if (myFile.fail()) {
		cout << "Podano bledna sciezke do pliku!" << endl;
	}
	else {
		DynamicProgramming::~DynamicProgramming();
		myFile >> numberOfCities;

		matrix = new int* [numberOfCities];
		for (int i = 0; i < numberOfCities; i++) matrix[i] = new int[numberOfCities];

		for (int i = 0; i < numberOfCities; i++)
			for (int j = 0; j < numberOfCities; j++) matrix[i][j] = 0;

		for (int i = 0; i < numberOfCities; i++) {
			for (int j = 0; j < numberOfCities; j++) {
				int value;
				myFile >> value;
				matrix[i][j] = value;
			}
		}
		myFile.close();
	}

	minPathLength = INT_MAX;
}

void DynamicProgramming::generateRandom(int numberOfC) {
	DynamicProgramming::~DynamicProgramming();
	numberOfCities = numberOfC;
	matrix = new int* [numberOfCities];
	for (int i = 0; i < numberOfCities; i++) matrix[i] = new int[numberOfCities];

	for (int i = 0; i < numberOfCities; i++) {
		for (int j = 0; j < numberOfCities; j++) {
			if (i == j) {
				matrix[i][j] = -1;
			}
			else {
				matrix[i][j] = generateRandomNumber(0, 20);
			}
		}
	}
	minPathLength = INT_MAX;
}

// Inicjalizuje tablice 'dp' oraz 'parent' do przechowywania wynik�w oblicze� oraz informacji o trasach.
void DynamicProgramming::initDPandParent() {
	// Alokacja pami�ci dla tablicy 'dp' oraz 'parent'.
	dp = new int* [numberOfCities];
	parent = new int* [numberOfCities];
	for (int i = 0; i < numberOfCities; i++) {
		dp[i] = new int[1 << numberOfCities];      // U�ycie przesuni�cia bitowego do alokacji pami�ci dla wszystkich kombinacji tras.
		parent[i] = new int[1 << numberOfCities];
		for (int j = 0; j < (1 << numberOfCities); j++) {
			dp[i][j] = -1;          // Inicjalizacja tablicy 'dp' warto�ciami -1, co oznacza, �e dana trasa nie zosta�a jeszcze obliczona.
			parent[i][j] = -1;      // Inicjalizacja tablicy 'parent' warto�ciami -1, co oznacza brak informacji o trasie.
		}
	}
}


void DynamicProgramming::solveTSP() {
	initDPandParent();     

	// Funkcja tsp korzysta z podej�cia dynamicznego do rozwi�zania problemu TSP.
	// Argumenty: i - obecne miasto, mask - binarny zestaw odwiedzonych miast.
	function<int(int, int)> tsp = [&](int i, int mask) {
		// Warunek bazowy: je�li wszystkie miasta zosta�y odwiedzone, zwr�� odleg�o�� powrotu do pocz�tkowego miasta.
		if (mask == (1 << numberOfCities) - 1) {
			return matrix[i][0];
		}
		// Je�li dana trasa zosta�a ju� obliczona, zwr�� zapisany wynik.
		if (dp[i][mask] != -1) {
			return dp[i][mask];
		}
		int ans = INT_MAX;          // Zmienna do przechowywania minimalnej odleg�o�ci.
		int bestNextCity = -1;      // Zmienna do przechowywania indeksu najlepszego nast�pnego miasta.
		for (int j = 0; j < numberOfCities; j++) {
			// Je�li miasto 'j' nie zosta�o odwiedzone (sprawdzenie za pomoc� operacji bitowej AND z mask�):
			if (!(mask & (1 << j))) {
				// Oblicz odleg�o�� dla obecnej trasy, dodaj�c odleg�o�� od 'i' do 'j' oraz rekurencyjnie obliczaj�c odleg�o�� dla nast�pnego miasta.
				int currCost = matrix[i][j] + tsp(j, mask | (1 << j));
				// Je�li obecna trasa jest kr�tsza ni� zapisana, zaktualizuj 'ans' i 'bestNextCity'.
				if (currCost < ans) {
					ans = currCost;
					bestNextCity = j;
				}
			}
		}
		parent[i][mask] = bestNextCity;     // Zapisz najlepsze nast�pne miasto dla obecnej trasy.
		return dp[i][mask] = ans;           // Zapisz i zwr�� minimaln� odleg�o�� dla obecnej trasy.
	};

	// Rozpocznij rozwi�zanie TSP z pocz�tkowego miasta (indeks 0) z mask� 1 (tylko pocz�tkowe miasto odwiedzone).
	minPathLength = tsp(0, 1);
}

