#pragma once
#include <string>

using namespace std;


class DynamicProgramming {
	int numberOfCities;
	int** matrix;

	int minPathLength;
	int** dp;  // Tablica do przechowywania wyników podproblemów
	int** parent;  // Tablica do rekonstrukcji œcie¿ki

public:
    DynamicProgramming();
    ~DynamicProgramming();
    int manageDynamicProgrammingMenu();
    void display();
    void loadFromFile(string filePath);
    void generateRandom(int numberOfVertices);
    void solveTSP();
    void initDPandParent();
};