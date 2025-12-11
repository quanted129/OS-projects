#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include "common.h"

using namespace std;

struct RecordLock
{
    int record_id;
    int readers;
    bool writing;
};

char filename[256];
vector<RecordLock> locks;
CRITICAL_SECTION cs_locks;

struct employee get_record(int id, bool& found)
{
    FILE* f = fopen(filename, "rb");
    struct employee e;
    found = false;
    if (!f) return e;

    while (fread(&e, sizeof(e), 1, f))
    {
        if (e.num == id)
        {
            found = true;
            break;
        }
    }
    fclose(f);
    return e;
}

void update_record(struct employee new_e)
{
    FILE* f = fopen(filename, "r+b");
    if (!f) return;
    struct employee e;
    while (fread(&e, sizeof(e), 1, f))
    {
        if (e.num == new_e.num)
        {
            fseek(f, -((long)sizeof(e)), SEEK_CUR);
            fwrite(&new_e, sizeof(e), 1, f);
            break;
        }
    }
    fclose(f);
}

void print_file()
{
    FILE* f = fopen(filename, "rb");
    if (!f) { cout << "File not found.\n"; return; }
    struct employee e;
    cout << "\nFile content:\n";
    cout << "ID\tName\tHours\n";
    while (fread(&e, sizeof(e), 1, f))
    {
        cout << e.num << "\t" << e.name << "\t" << e.hours << endl;
    }
    cout << "-----------------------\n";
    fclose(f);
}

bool acquire_lock(int id, bool for_write)
{
    EnterCriticalSection(&cs_locks);

    auto it = std::find_if(locks.begin(), locks.end(), [id](const RecordLock& l) { return l.record_id == id; });
    
    if (it == locks.end())
    {
        locks.push_back({ id, 0, false });
        it = locks.end() - 1;
    }

    bool can_access = false;
    if (for_write)
    {
        if (it->readers == 0 && !it->writing)
        {
            it->writing = true;
            can_access = true;
        }
    }
    else
    {
        if (!it->writing)
        {
            it->readers++;
            can_access = true;
        }
    }

    LeaveCriticalSection(&cs_locks);
    return can_access;
}

void release_lock(int id, bool for_write)
{
    EnterCriticalSection(&cs_locks);
    auto it = std::find_if(locks.begin(), locks.end(), [id](const RecordLock& l) { return l.record_id == id; });
    if (it != locks.end())
    {
        if (for_write) it->writing = false;
        else it->readers--;
    }
    LeaveCriticalSection(&cs_locks);
}

DWORD WINAPI ClientHandler(LPVOID lpParam)
{
    HANDLE hPipe = (HANDLE)lpParam;
    Message msg;
    DWORD bytesRead, bytesWritten;
    bool connected = true;
    int current_locked_id = -1;
    bool current_is_write_lock = false;

    while (connected)
    {
        if (ReadFile(hPipe, &msg, sizeof(msg), &bytesRead, NULL))
        {
            Message response;
            response.command = msg.command;
            response.success = true;

            if (msg.command == CMD_READ || msg.command == CMD_MODIFY)
            {
                bool is_write = (msg.command == CMD_MODIFY);

                while (!acquire_lock(msg.record_id, is_write))
                {
                    Sleep(50); 
                }
                
                current_locked_id = msg.record_id;
                current_is_write_lock = is_write;

                bool found;
                response.data = get_record(msg.record_id, found);
                if (!found)
                {
                    response.success = false;
                    strcpy(response.error_msg, "ID not found");
                    release_lock(current_locked_id, current_is_write_lock);
                    current_locked_id = -1;
                }
                else
                {
                    response.record_id = msg.record_id;
                }
            }
            else if (msg.command == CMD_SAVE)
            {
                if (current_locked_id == msg.data.num && current_is_write_lock)
                {
                    update_record(msg.data);
                }
                else
                {
                    response.success = false;
                }
            }
            else if (msg.command == CMD_FINISH)
            {
                if (current_locked_id != -1)
                {
                    release_lock(current_locked_id, current_is_write_lock);
                    current_locked_id = -1;
                }
            }
            else if (msg.command == CMD_EXIT)
            {
                connected = false;
            }
            WriteFile(hPipe, &response, sizeof(response), &bytesWritten, NULL);
        }
        else
        {
            connected = false;
        }
    }

    if (current_locked_id != -1)
    {
        release_lock(current_locked_id, current_is_write_lock);
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 0;
}

int main()
{
    InitializeCriticalSection(&cs_locks);

    cout << "Enter filename: ";
    cin >> filename;

    int n;
    cout << "Enter number of employees: ";
    cin >> n;

    FILE* f = fopen(filename, "wb");
    for (int i = 0; i < n; i++)
    {
        struct employee e;
        cout << "Employee " << i + 1 << " (ID Name Hours): ";
        cin >> e.num >> e.name >> e.hours;
        fwrite(&e, sizeof(e), 1, f);
    }
    fclose(f);

    print_file();

    int clientCount;
    cout << "Enter number of clients to launch: ";
    cin >> clientCount;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    char cmdLine[] = "Client.exe"; 

    for (int i = 0; i < clientCount; i++)
    {
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
            cout << "Failed to launch client. Error: " << GetLastError() << endl;
        }
        else
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    cout << "Server is running. Waiting for connections...\n";

    for (int i = 0; i < clientCount; i++)
    {
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            sizeof(Message), sizeof(Message),
            0, NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            cout << "CreateNamedPipe failed.\n";
            continue;
        }

        if (ConnectNamedPipe(hPipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED)
        {
            CreateThread(NULL, 0, ClientHandler, (LPVOID)hPipe, 0, NULL);
        }
        else
        {
            CloseHandle(hPipe);
        }
    }

    cout << "Press any key to finish server and display file...\n";
    _getch();

    print_file();
    DeleteCriticalSection(&cs_locks);
    return 0;
}