#pragma once

#include <windows.h>
#include <string>
#include <iostream>

struct QueueHeader
{
    DWORD head;
    DWORD tail;
    DWORD size;
    bool initialized;
};

class MessageQueue
{
private:
    HANDLE hMapFile;
    LPCTSTR pBuf;
    HANDLE hMutex;
    HANDLE hNotEmpty;
    HANDLE hNotFull;
    QueueHeader* pHeader;
    char* messages;
    std::string queueName;

public:
    MessageQueue() : hMapFile(NULL), pBuf(NULL), hMutex(NULL), hNotEmpty(NULL), hNotFull(NULL), pHeader(nullptr), messages(nullptr) {}

    ~MessageQueue() { Cleanup(); }

    bool Create(const std::string& fileName, DWORD queueSize)
    {
        queueName = fileName;

        hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(QueueHeader) + queueSize * 20,
            fileName.c_str()
        );

        if (hMapFile == NULL) return false;

        pBuf = (LPTSTR)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(QueueHeader) + queueSize * 20
        );

        if (pBuf == NULL)
        {
            CloseHandle(hMapFile);
            return false;
        }

        pHeader = (QueueHeader*)pBuf;
        pHeader->head = 0;
        pHeader->tail = 0;
        pHeader->size = queueSize;
        pHeader->initialized = true;
        messages = (char*)(pHeader + 1);

        hMutex = CreateMutex(NULL, FALSE, (fileName + "_mutex").c_str());
        hNotEmpty = CreateEvent(NULL, FALSE, FALSE, (fileName + "_notEmpty").c_str());
        hNotFull = CreateEvent(NULL, FALSE, TRUE, (fileName + "_notFull").c_str());

        return (hMutex != NULL && hNotEmpty != NULL && hNotFull != NULL);
    }

    bool Open(const std::string& fileName)
    {
        queueName = fileName;

        hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName.c_str());
        if (hMapFile == NULL) return false;

        pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (pBuf == NULL)
        {
            CloseHandle(hMapFile);
            return false;
        }

        pHeader = (QueueHeader*)pBuf;
        messages = (char*)(pHeader + 1);

        hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (fileName + "_mutex").c_str());
        hNotEmpty = OpenEvent(EVENT_ALL_ACCESS, FALSE, (fileName + "_notEmpty").c_str());
        hNotFull = OpenEvent(EVENT_ALL_ACCESS, FALSE, (fileName + "_notFull").c_str());

        return (hMutex != NULL && hNotEmpty != NULL && hNotFull != NULL);
    }

    bool SendMessage(const std::string& message)
    {
        WaitForSingleObject(hMutex, INFINITE);

        DWORD nextTail = (pHeader->tail + 1) % pHeader->size;
        if (nextTail == pHeader->head)
        {
            ResetEvent(hNotFull);
            ReleaseMutex(hMutex);
            WaitForSingleObject(hNotFull, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);
            nextTail = (pHeader->tail + 1) % pHeader->size;
        }

        std::string truncated = message.substr(0, 20);
        memcpy(messages + pHeader->tail * 20, truncated.c_str(), truncated.length());
        pHeader->tail = nextTail;

        SetEvent(hNotEmpty);
        ReleaseMutex(hMutex);
        return true;
    }

    bool ReceiveMessage(std::string& message)
    {
        WaitForSingleObject(hMutex, INFINITE);

        if (pHeader->head == pHeader->tail)
        {
            ResetEvent(hNotEmpty);
            ReleaseMutex(hMutex);
            WaitForSingleObject(hNotEmpty, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);
        }

        char buffer[21] = {0};
        memcpy(buffer, messages + pHeader->head * 20, 20);
        pHeader->head = (pHeader->head + 1) % pHeader->size;
        message = buffer;

        SetEvent(hNotFull);
        ReleaseMutex(hMutex);
        return true;
    }

    bool IsEmpty() const
    {
        return pHeader->head == pHeader->tail;
    }

    bool IsFull() const
    {
        return ((pHeader->tail + 1) % pHeader->size) == pHeader->head;
    }

    void Cleanup()
    {
        if (pBuf) UnmapViewOfFile(pBuf);
        if (hMapFile) CloseHandle(hMapFile);
        if (hMutex) CloseHandle(hMutex);
        if (hNotEmpty) CloseHandle(hNotEmpty);
        if (hNotFull) CloseHandle(hNotFull);

        pBuf = NULL;
        hMapFile = NULL;
        hMutex = NULL;
        hNotEmpty = NULL;
        hNotFull = NULL;
        pHeader = nullptr;
        messages = nullptr;
    }

    void SignalReady()
    {
        SetEvent(hNotFull);
    }
};