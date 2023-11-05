#pragma once
#include "BruteForce.h"
#include "Random.h"
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

BruteForce::BruteForce() {
	numberOfCities = 0;
	matrix = NULL;
	minPathLength = INT_MAX;
	bestPath = NULL;
	initialPath = NULL;
	visited = NULL;
}

BruteForce::~BruteForce() {
	if (numberOfCities != 0) {
		for (int i = 0; i < numberOfCities; i++) delete[] matrix[i];
		delete[] matrix;
		delete[] bestPath;
		delete[] initialPath;
		delete[] visited;
		numberOfCities = 0;
	}
}

int BruteForce::manageBruteForceMenu() {
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
		visited[0] = true;
		initialPath[0] = 0;
		for (int i = 1; i < numberOfCities;i++) {
			visited[i] = false;
		}

		start = steady_clock::now();
		algorithm(1, initialPath);
		duration = steady_clock::now() - start;
		time = duration_cast<microseconds>(duration).count();
		cout << "Czas wykonania: " << time << "microseconds" << endl;

		cout << "Najlepsza trasa: ";
		for (int i = 0; i < numberOfCities; i++) {
			cout << bestPath[i] << "->";
		}
		cout << bestPath[0];
		cout << endl;
		cout << "Dlugosc trasy: " << minPathLength << endl;
		break;
	default:
		BruteForce::~BruteForce();
		break;
	}
	return choice;

}

void BruteForce::display() {
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

void BruteForce::loadFromFile(string path) {
	ifstream  myFile(path);
	if (myFile.fail()) {
		cout << "Podano bledna sciezke do pliku!" << endl;
	}
	else {
		BruteForce::~BruteForce();
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
	visited = new bool[numberOfCities];
	bestPath = new int[numberOfCities];
	initialPath = new int[numberOfCities];
	minPathLength = INT_MAX;
}

void BruteForce::generateRandom(int numberOfC) {
	BruteForce::~BruteForce();
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
	visited = new bool[numberOfCities];
	bestPath = new int[numberOfCities];
	initialPath = new int[numberOfCities];
	minPathLength = INT_MAX;
}

void BruteForce::algorithm(int depth, int* path) {
	if (depth == numberOfCities) {
		calculateBestPath(path);
		return;
	}

	for (int i = 0; i < numberOfCities; i++) {
		if (!visited[i]) {
			visited[i] = true;
			path[depth] = i;
			algorithm(depth + 1, path);
			visited[i] = false;
		}
	}
}


void BruteForce::calculateBestPath(int* path) {
	int length = 0;
	for (int i = 0; i < numberOfCities - 1; i++) {
		length += matrix[path[i]][path[i + 1]];
	}
	length += matrix[path[numberOfCities - 1]][path[0]];  // Powrót do pocz¹tkowego miasta
	if (length < minPathLength) {
		minPathLength = length;
		for (int i = 0; i < numberOfCities; i++) {
			bestPath[i] = path[i];
		}
	}
}

