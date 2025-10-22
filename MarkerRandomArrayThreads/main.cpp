#include <iostream>
#include <vector>
#include <windows.h>
#include "header.h"
using namespace std;

int main()
{
    int size;
    cout << "Enter array size: ";
    cin >> size;
    vector<int> array(size, 0);
    int markerCount;
    cout << "Enter number of marker threads: ";
    cin >> markerCount;
    cout << '\n';

    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);
    CRITICAL_SECTION outputCriticalSection;
    InitializeCriticalSection(&outputCriticalSection);

    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    vector<ThreadData> threadsData(markerCount);
    vector<HANDLE> threads(markerCount);
    vector<HANDLE> threadHandles(markerCount);

    for (int i = 0; i < markerCount; i++)
    {
        threadsData[i] = { i + 1, array.data(), size, startEvent, CreateEvent(NULL, FALSE, FALSE, NULL), CreateEvent(NULL, FALSE, FALSE, NULL), CreateEvent(NULL, FALSE, FALSE, NULL), &criticalSection, &outputCriticalSection };
        threads[i] = CreateThread(NULL, 0, MarkerThread, &threadsData[i], 0, NULL);
        threadHandles[i] = threads[i];
    }

    SetEvent(startEvent);

    int activeMarkers = markerCount;
    while (activeMarkers > 0)
    {
        vector<HANDLE> activeReadyEvents;
        vector<int> activeIndices;

        for (int i = 0; i < markerCount; i++)
        {
            if (!threadsData[i].isTerminated)
            {
                activeReadyEvents.push_back(threadsData[i].threadReady);
                activeIndices.push_back(i);
            }
        }

        if (activeReadyEvents.empty()) break;

        WaitForMultipleObjects((activeReadyEvents.size()), activeReadyEvents.data(), TRUE, INFINITE);

        for (HANDLE event : activeReadyEvents) ResetEvent(event);

        EnterCriticalSection(&criticalSection);
        cout << "\nCurrent array: ";
        for (int i = 0; i < size; i++) cout << array[i] << " ";
        cout << "\n\n";
        LeaveCriticalSection(&criticalSection);

        int markerToTerminate;
        cout << "Enter marker ID to terminate: ";
        cin >> markerToTerminate;

        int terminateIndex = -1;
        for (int i = 0; i < markerCount; i++)
        {
            if (threadsData[i].markerId == markerToTerminate && !threadsData[i].isTerminated)
            {
                terminateIndex = i;
                break;
            }
        }

        if (terminateIndex == -1)
        {
            cout << "Invalid marker ID entered!" << endl;
            for (int i = 0; i < markerCount; i++)
            {
                if (!threadsData[i].isTerminated)
                {
                    SetEvent(threadsData[i].continueEvent);
                }
            }
            continue;
        }

        SetEvent(threadsData[terminateIndex].terminateEvent);

        DWORD waitResult = WaitForSingleObject(threads[terminateIndex], 5000);
        if (waitResult == WAIT_TIMEOUT)
        {
            cout << "Warning: Thread " << markerToTerminate << " did not terminate in time!" << endl;
            TerminateThread(threads[terminateIndex], 0);
        }

        threadsData[terminateIndex].isTerminated = 1;
        activeMarkers--;

        EnterCriticalSection(&criticalSection);
        cout << "\nArray after termination of marker " << markerToTerminate << ": ";
        for (int i = 0; i < size; i++) cout << array[i] << " ";
        cout << "\n\n";
        LeaveCriticalSection(&criticalSection);

        for (int i = 0; i < markerCount; i++)
        {
            if (!threadsData[i].isTerminated)
            {
                SetEvent(threadsData[i].continueEvent);
            }
        }
    }

    DeleteCriticalSection(&criticalSection);
    DeleteCriticalSection(&outputCriticalSection);
    CloseHandle(startEvent);

    for (int i = 0; i < markerCount; i++)
    {
        CloseHandle(threads[i]);
        CloseHandle(threadsData[i].continueEvent);
        CloseHandle(threadsData[i].terminateEvent);
        CloseHandle(threadsData[i].threadReady);
    }
    return 0;
}