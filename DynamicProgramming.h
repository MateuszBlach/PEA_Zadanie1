#pragma once
#include <string>

using namespace std;


class DynamicProgramming {
	int numberOfCities;
	int** matrix;

	int minPathLength;
	int** dp;  // Tablica do przechowywania wynik�w podproblem�w
	int** parent;  // Tablica do rekonstrukcji �cie�ki

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