#include <iostream>
#include "BruteForce.h"
#include "DynamicProgramming.h"

using namespace std;

int mainMenuChoice;
int problemMenuChoice;

BruteForce bruteForce;
DynamicProgramming dynamicProgramming;

void displayMainMenu();
void manageMainMenu();
void displayProblemMenu();

int main()
{
	do {
		displayMainMenu();
		manageMainMenu();
	} while (mainMenuChoice != 0);

}

void displayMainMenu() {
	cout << "Wybierz algorytm rozwiazania asymetrycznego problemu komiwojazera:" << endl;
	cout << "1. Przeglad Zupelny - Brute Force" << endl;
	cout << "2. Programowanie Dynamiczne " << endl;
	cout << "0. Zakoncz dzialanie programu" << endl;
	cout << "Wybor: ";
}

void manageMainMenu() {
	cin >> mainMenuChoice;
	switch (mainMenuChoice) {
	case 1:
		bruteForce = BruteForce();
		srand(time(NULL));
		do {
			displayProblemMenu();
			problemMenuChoice = bruteForce.manageBruteForceMenu();
		} while (problemMenuChoice != 0);
		bruteForce.~BruteForce();
		break;
	case 2:
		dynamicProgramming = DynamicProgramming();
		srand(time(NULL));
		do {
			displayProblemMenu();
			problemMenuChoice = dynamicProgramming.manageDynamicProgrammingMenu();
		} while (problemMenuChoice != 0);
		dynamicProgramming.~DynamicProgramming();
		break;
	default:
		break;
	}
}

void displayProblemMenu() {
	cout << "Wybierz dzialanie:" << endl;
	cout << "1. Wczytaj z pliku" << endl;
	cout << "2. Wygeneruj losowo" << endl;
	cout << "3. Wyświetl" << endl;
	cout << "4. Algorytm" << endl;
	cout << "0. Powrot do menu glownego" << endl;
	cout << "Wybor: ";
}


