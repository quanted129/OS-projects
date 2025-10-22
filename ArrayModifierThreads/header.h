#pragma once

#include <windows.h>
#include <vector>
using namespace std;

struct ArrayValue
{
    int* array;
    int size;
    int min;
    int max;
    double averageValue;
    vector<int> minIndexes;
    vector<int> maxIndexes;
    
    ArrayValue();
    explicit ArrayValue(int arrSize);
    ~ArrayValue();
    
    void initializeMinMax();
    void replaceMinMaxWithAverage();
};

DWORD WINAPI MinMaxThread(LPVOID lpParam);
DWORD WINAPI AverageThread(LPVOID lpParam);

void findMinMax(ArrayValue* data);
void calculateAverage(ArrayValue* data);