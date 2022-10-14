#ifndef COMMONFUNCTIONS
#define COMMONFUNCTIONS

#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <list>

void putSCCNumAndSize(int n, int *p);

void putProcess(double procedure, unsigned long long time_used);

std::stringstream timeFormatting(unsigned long long seconds);

unsigned long long currentTime();

#endif