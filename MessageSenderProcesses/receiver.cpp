#include "header.h"
#include <vector>

int main()
{
    std::string fileName;
    DWORD queueSize, numSenders;

    std::cout << "Enter binary file name: ";
    std::cin >> fileName;
    std::cout << "Enter queue size: ";
    std::cin >> queueSize;
    std::cout << "Enter number of Sender processes: ";
    std::cin >> numSenders;

    MessageQueue queue;
    queue.Create(fileName, ++queueSize);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    std::vector<PROCESS_INFORMATION> processes;

    for (DWORD i = 0; i < numSenders; i++)
    {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string cmd = "cmd.exe /c start Sender.exe " + fileName;
        CreateProcess(NULL, const_cast<LPSTR>(cmd.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        processes.push_back(pi);
    }

    for (DWORD i = 0; i < numSenders; i++)
    {
        WaitForSingleObject(OpenEvent(EVENT_ALL_ACCESS, FALSE, (fileName + "_notFull").c_str()), INFINITE);
    }

    std::string command;
    bool run = 1;
    while (run)
    {
        std::cout << "Enter command (read/exit): ";
        std::cin >> command;

        if (command == "read")
        {
            std::string message;
            if (queue.ReceiveMessage(message))
            {
                std::cout << "Received message: " << message << std::endl;
            }
        }
        else if (command == "exit")
        {
            run = 0;
        }
    }

    for (auto& proc : processes)
    {
        TerminateProcess(proc.hProcess, 0);
        CloseHandle(proc.hProcess);
        CloseHandle(proc.hThread);
    }

    return 0;
}