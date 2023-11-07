#pragma once
#include <string>

using namespace std;


class BruteForce {
	int numberOfCities;
	int** matrix;
	int* initialPath;
	int* bestPath;
	int minPathLength;
	bool* visited;

public:
	BruteForce();
	~BruteForce();
	int manageBruteForceMenu();
	void display();
	void loadFromFile(string filePath);
	void generateRandom(int numberOfVertices);
	void algorithm(int depth, int* path);
	void calculateBestPath(int* path);
};