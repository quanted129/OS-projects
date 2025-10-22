#include <gtest/gtest.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include "../header.h"
using namespace std;

TEST(MarkerThreadTest, ThreadTermination)
{
    int size = 10;
    vector<int> array(size, 0);

    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);
    CRITICAL_SECTION outputCriticalSection;
    InitializeCriticalSection(&outputCriticalSection);

    ThreadData data = { 1, array.data(), size, CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), &criticalSection, &outputCriticalSection };
    HANDLE thread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);

    SetEvent(data.startEvent);
    SetEvent(data.terminateEvent);

    DWORD result = WaitForSingleObject(thread, 5000);
    EXPECT_EQ(result, WAIT_OBJECT_0);

    for (int i = 0; i < size; i++) EXPECT_EQ(array[i], 0);

    CloseHandle(thread);
    CloseHandle(data.startEvent);
    CloseHandle(data.continueEvent);
    CloseHandle(data.terminateEvent);
    CloseHandle(data.threadReady);
    DeleteCriticalSection(&criticalSection);
    DeleteCriticalSection(&outputCriticalSection);
}

TEST(MarkerThreadTest, MarkerWritesToArray)
{
    int size = 5;
    vector<int> array(size, 0);

    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);
    CRITICAL_SECTION outputCriticalSection;
    InitializeCriticalSection(&outputCriticalSection);

    ThreadData data = { 1, array.data(), size, CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), CreateEvent(NULL, TRUE, FALSE, NULL), &criticalSection, &outputCriticalSection };

    HANDLE thread = CreateThread(NULL, 0, MarkerThread, &data, 0, NULL);

    SetEvent(data.startEvent);
    Sleep(100);

    SetEvent(data.terminateEvent);
    WaitForSingleObject(thread, 5000);

    for (int i = 0; i < size; i++) EXPECT_EQ(array[i], 0);

    CloseHandle(thread);
    CloseHandle(data.startEvent);
    CloseHandle(data.continueEvent);
    CloseHandle(data.terminateEvent);
    CloseHandle(data.threadReady);
    DeleteCriticalSection(&criticalSection);
    DeleteCriticalSection(&outputCriticalSection);
}