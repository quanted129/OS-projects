#include "header.h"
#include <iostream>
using namespace std;

int main()
{
    int size;
    cout << "Enter array size: ";
    cin >> size;
    if (size < 1)
    {
        cout << "Size must be positive!" << endl;
        return 1;
    }
    ArrayValue arr(size);
    for (int i = 0; i < size; i++)
    {
        cout << "Element " << i + 1 << ": ";
        cin >> arr.array[i];
    }

    HANDLE hMinMaxThread = CreateThread(NULL, 0, MinMaxThread, &arr, 0, NULL);
    HANDLE hAverageThread = CreateThread(NULL, 0, AverageThread, &arr, 0, NULL);

    HANDLE threads[2] = { hMinMaxThread, hAverageThread };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(hMinMaxThread);
    CloseHandle(hAverageThread);

    arr.replaceMinMaxWithAverage();
    cout << "\nMin element: " << arr.min << endl;
    cout << "Max element: " << arr.max << endl;
    cout << "Average value: " << arr.averageValue << " (applied as " << static_cast<int>(arr.averageValue) << ")\n";
    cout << "New array: { ";
    for (int i = 0; i < size; i++)cout << arr.array[i] << ' ';
    cout << "}\n";
    return 0;
}