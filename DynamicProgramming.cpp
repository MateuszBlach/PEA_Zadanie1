#pragma once
#include "DynamicProgramming.h"
#include "Random.h"
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
	int choice, numberOfV;
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
				matrix[i][j] = NO_CONNECTION;
			}
			else {
				matrix[i][j] = generateRandomNumber(0, 20);
			}
		}
	}
	minPathLength = INT_MAX;
}

void DynamicProgramming::initDPandParent() {
	dp = new int* [numberOfCities];
	parent = new int* [numberOfCities];
	for (int i = 0; i < numberOfCities; i++) {
		dp[i] = new int[1 << numberOfCities];
		parent[i] = new int[1 << numberOfCities];
		for (int j = 0; j < (1 << numberOfCities); j++) {
			dp[i][j] = -1;
			parent[i][j] = -1;
		}
	}
}

void DynamicProgramming::solveTSP() {
	initDPandParent();

	std::function<int(int, int)> tsp = [&](int i, int mask) {
		if (mask == (1 << numberOfCities) - 1) {
			return matrix[i][0];
		}
		if (dp[i][mask] != -1) {
			return dp[i][mask];
		}
		int ans = INT_MAX;
		int bestNextCity = -1;
		for (int j = 0; j < numberOfCities; j++) {
			if (!(mask & (1 << j))) {
				int currCost = matrix[i][j] + tsp(j, mask | (1 << j));
				if (currCost < ans) {
					ans = currCost;
					bestNextCity = j;
				}
			}
		}
		parent[i][mask] = bestNextCity;
		return dp[i][mask] = ans;
	};

	minPathLength = tsp(0, 1);

	// Odtworzenie œcie¿ki i wydrukowanie jej
	cout << "Najlepsza trasa: ";
	int currCity = 0;
	int mask = 1;
	for (int i = 0; i < numberOfCities - 1; i++) {
		cout << currCity << " -> ";
		currCity = parent[currCity][mask];
		mask |= (1 << currCity);
	}
	cout << currCity << " -> 0" << endl;
}

