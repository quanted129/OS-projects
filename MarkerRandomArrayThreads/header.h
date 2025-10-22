#pragma once
#include <windows.h>

struct ThreadData
{
    int markerId;
    int* array;
    int arraySize;
    HANDLE startEvent;
    HANDLE continueEvent;
    HANDLE terminateEvent;
    HANDLE threadReady;
    CRITICAL_SECTION* criticalSection;
    CRITICAL_SECTION* outputCriticalSection;
    bool isTerminated = 0;
    int markedCount = 0;
};

DWORD WINAPI MarkerThread(LPVOID lpParam);