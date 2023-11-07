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

// Inicjalizuje tablice 'dp' oraz 'parent' do przechowywania wyników obliczeñ oraz informacji o trasach.
void DynamicProgramming::initDPandParent() {
	// Alokacja pamiêci dla tablicy 'dp' oraz 'parent'.
	dp = new int* [numberOfCities];
	parent = new int* [numberOfCities];
	for (int i = 0; i < numberOfCities; i++) {
		dp[i] = new int[1 << numberOfCities];      // U¿ycie przesuniêcia bitowego do alokacji pamiêci dla wszystkich kombinacji tras.
		parent[i] = new int[1 << numberOfCities];
		for (int j = 0; j < (1 << numberOfCities); j++) {
			dp[i][j] = -1;          // Inicjalizacja tablicy 'dp' wartoœciami -1, co oznacza, ¿e dana trasa nie zosta³a jeszcze obliczona.
			parent[i][j] = -1;      // Inicjalizacja tablicy 'parent' wartoœciami -1, co oznacza brak informacji o trasie.
		}
	}
}


void DynamicProgramming::solveTSP() {
	initDPandParent();     

	// Funkcja tsp korzysta z podejœcia dynamicznego do rozwi¹zania problemu TSP.
	// Argumenty: i - obecne miasto, mask - binarny zestaw odwiedzonych miast.
	function<int(int, int)> tsp = [&](int i, int mask) {
		// Warunek bazowy: jeœli wszystkie miasta zosta³y odwiedzone, zwróæ odleg³oœæ powrotu do pocz¹tkowego miasta.
		if (mask == (1 << numberOfCities) - 1) {
			return matrix[i][0];
		}
		// Jeœli dana trasa zosta³a ju¿ obliczona, zwróæ zapisany wynik.
		if (dp[i][mask] != -1) {
			return dp[i][mask];
		}
		int ans = INT_MAX;          // Zmienna do przechowywania minimalnej odleg³oœci.
		int bestNextCity = -1;      // Zmienna do przechowywania indeksu najlepszego nastêpnego miasta.
		for (int j = 0; j < numberOfCities; j++) {
			// Jeœli miasto 'j' nie zosta³o odwiedzone (sprawdzenie za pomoc¹ operacji bitowej AND z mask¹):
			if (!(mask & (1 << j))) {
				// Oblicz odleg³oœæ dla obecnej trasy, dodaj¹c odleg³oœæ od 'i' do 'j' oraz rekurencyjnie obliczaj¹c odleg³oœæ dla nastêpnego miasta.
				int currCost = matrix[i][j] + tsp(j, mask | (1 << j));
				// Jeœli obecna trasa jest krótsza ni¿ zapisana, zaktualizuj 'ans' i 'bestNextCity'.
				if (currCost < ans) {
					ans = currCost;
					bestNextCity = j;
				}
			}
		}
		parent[i][mask] = bestNextCity;     // Zapisz najlepsze nastêpne miasto dla obecnej trasy.
		return dp[i][mask] = ans;           // Zapisz i zwróæ minimaln¹ odleg³oœæ dla obecnej trasy.
	};

	// Rozpocznij rozwi¹zanie TSP z pocz¹tkowego miasta (indeks 0) z mask¹ 1 (tylko pocz¹tkowe miasto odwiedzone).
	minPathLength = tsp(0, 1);
}

