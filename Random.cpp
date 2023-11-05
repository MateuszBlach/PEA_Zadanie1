#include <cstdlib>
#include <ctime>

using namespace std;

int generateRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}