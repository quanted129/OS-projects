#include "header.h"
#include <iostream>
#include <climits>
using namespace std;

ArrayValue::ArrayValue() : array(nullptr), size(0), min(INT_MAX), max(INT_MIN), averageValue(0) {}

ArrayValue::ArrayValue(int arrSize) : array(new int[arrSize]), size(arrSize), min(INT_MAX), max(INT_MIN), averageValue(0) {}

ArrayValue::~ArrayValue() { delete[] array; }


void ArrayValue::initializeMinMax()
{
    min = INT_MAX;
    max = INT_MIN;
}

void ArrayValue::replaceMinMaxWithAverage()
{
    if (!minIndexes.empty() && !maxIndexes.empty())
    {
        int replacementValue = static_cast<int>(averageValue);
        for (int i : minIndexes) array[i] = replacementValue;
        for (int i : maxIndexes) array[i] = replacementValue;
    }
}

void findMinMax(ArrayValue* data)
{
    int* arr = data->array;
    int arrSize = data->size;
    if (arrSize == 0) return;

    data->initializeMinMax();
    data->minIndexes.clear();
    data->maxIndexes.clear();

    data->min = arr[0];
    data->max = arr[0];
    data->minIndexes.push_back(0);
    data->maxIndexes.push_back(0);

    for (int i = 1; i < arrSize; i++)
    {
        if (arr[i] < data->min)
        {
            data->min = arr[i];
            data->minIndexes.clear();
            data->minIndexes.push_back(i);
        }
        else if (arr[i] == data->min)
        {
            data->minIndexes.push_back(i);
        }
        Sleep(7);

        if (arr[i] > data->max)
        {
            data->max = arr[i];
            data->maxIndexes.clear();
            data->maxIndexes.push_back(i);
        }
        else if (arr[i] == data->max)
        {
            data->maxIndexes.push_back(i);
        }
        Sleep(7);
    }
}

void calculateAverage(ArrayValue* data)
{
    int* arr = data->array;
    int arrSize = data->size;

    if (!arrSize)
    {
        data->averageValue = 0;
        return;
    }

    double sum = 0;
    for (int i = 0; i < arrSize; i++)
    {
        sum += arr[i];
        Sleep(12);
    }

    data->averageValue = sum / arrSize;
}

DWORD WINAPI MinMaxThread(LPVOID lpParam)
{
    ArrayValue* data = static_cast<ArrayValue*>(lpParam);
    findMinMax(data);
    cout << "Found min (" << data->min << ") and max (" << data->max << ").\n";
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam)
{
    ArrayValue* data = static_cast<ArrayValue*>(lpParam);
    calculateAverage(data);
    cout << "Found average: " << data->averageValue << endl;
    return 0;
}