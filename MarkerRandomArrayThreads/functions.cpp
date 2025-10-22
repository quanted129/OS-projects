#include "header.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

DWORD WINAPI MarkerThread(LPVOID lpParam)
{
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    WaitForSingleObject(data->startEvent, INFINITE);
    srand(static_cast<unsigned int>(time(NULL)) + data->markerId);

    while (true)
    {
        int index = rand() % data->arraySize;
        EnterCriticalSection(data->criticalSection);
        if (!data->array[index])
        {
            Sleep(5);
            data->array[index] = data->markerId;
            data->markedCount++;
            Sleep(5);
            LeaveCriticalSection(data->criticalSection);
        }
        else
        {
            EnterCriticalSection(data->outputCriticalSection);
            cout << "Marker ID " << data->markerId << "; marked " << data->markedCount << " elements; can't mark arr[" << index << "]\n";
            LeaveCriticalSection(data->outputCriticalSection);
            LeaveCriticalSection(data->criticalSection);

            SetEvent(data->threadReady);
            HANDLE events[2] = { data->continueEvent, data->terminateEvent };
            DWORD result = WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (result == WAIT_OBJECT_0 + 1)
            {
                EnterCriticalSection(data->criticalSection);
                for (int i = 0; i < data->arraySize; i++)
                {
                    if (data->array[i] == data->markerId)
                    {
                        data->array[i] = 0;
                    }
                }
                LeaveCriticalSection(data->criticalSection);
                data->isTerminated = 1;
                return 0;
            }
            ResetEvent(data->continueEvent);
        }
    }
    return 0;
}