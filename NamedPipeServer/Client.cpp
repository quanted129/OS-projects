#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "common.h"

using namespace std;

void print_employee(const struct employee& e)
{
    cout << "ID: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << endl;
}

int main()
{
    Sleep(1000); 

    HANDLE hPipe = CreateFile(
        PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << "Could not connect to server. Error: " << GetLastError() << endl;
        system("pause");
        return 1;
    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

    while (true)
    {
        cout << "\nMenu:\n1. Modify record\n2. Read record\n3. Exit\nChoice: ";
        int choice;
        cin >> choice;

        if (choice == 3)
        {
            Message msg;
            msg.command = CMD_EXIT;
            DWORD bytesWritten;
            WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
            break;
        }

        int id;
        cout << "Enter Employee ID: ";
        cin >> id;

        Message req, resp;
        req.command = (choice == 1) ? CMD_MODIFY : CMD_READ;
        req.record_id = id;

        DWORD rw;
        if (!WriteFile(hPipe, &req, sizeof(req), &rw, NULL))
        {
            cout << "Write failed.\n"; break;
        }

        if (!ReadFile(hPipe, &resp, sizeof(resp), &rw, NULL))
        {
            cout << "Read failed.\n"; break;
        }

        if (!resp.success)
        {
            cout << "Server error: " << resp.error_msg << endl;
            continue;
        }

        cout << "Received record: ";
        print_employee(resp.data);

        if (choice == 1)
        {
            cout << "Enter new Name and Hours: ";
            cin >> resp.data.name >> resp.data.hours;
            cout << "Press any key to send update...";
            _getch();
            cout << endl;
            req.command = CMD_SAVE;
            req.data = resp.data;
            WriteFile(hPipe, &req, sizeof(req), &rw, NULL);
            ReadFile(hPipe, &resp, sizeof(resp), &rw, NULL);
            if(resp.success) cout << "Updated successfully.\n";
            else cout << "Update failed.\n";
            cout << "Press any key to finish access...";
            _getch();
            cout << endl;
        }
        else
        {
            cout << "Press any key to finish reading...";
            _getch();
            cout << endl;
        }

        req.command = CMD_FINISH;
        WriteFile(hPipe, &req, sizeof(req), &rw, NULL);
        ReadFile(hPipe, &resp, sizeof(resp), &rw, NULL);
    }

    CloseHandle(hPipe);
    return 0;
}