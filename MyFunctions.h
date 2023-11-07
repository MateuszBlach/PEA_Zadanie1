#pragma once
#include<chrono>
using namespace std;

int generateRandomNumber(int min, int max);

long getHumanReadableTime(std::chrono::steady_clock::duration duration);
