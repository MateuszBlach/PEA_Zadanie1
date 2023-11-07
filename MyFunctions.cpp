#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

int generateRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void getHumanReadableTime(std::chrono::steady_clock::duration duration) {
    

    auto secondss = duration_cast<seconds>(duration);
    duration -= secondss;

    auto millisecondss = duration_cast<milliseconds>(duration);
    duration -= millisecondss;

    auto microsecondss = duration_cast<microseconds>(duration);
    duration -= microsecondss;

    auto nanosecondss = duration_cast<nanoseconds>(duration);

    if (secondss.count() > 0) {
        cout << secondss.count() << "s ";
    }

    if (millisecondss.count() > 0) {
        cout << millisecondss.count() << "ms ";
    }

    if (microsecondss.count() > 0) {
        cout << microsecondss.count() << "us ";  
    }

    if (nanosecondss.count() > 0) {
        cout << nanosecondss.count() << "ns";
    }
}