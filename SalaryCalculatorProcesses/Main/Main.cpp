#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "header.h"
using namespace std;

void printBinaryFile(string filename)
{
    ifstream file(filename, ios::binary);
    employee emp;
    cout << "\nBinary file content:" << endl;

    while (file.read((char*)&emp, sizeof(employee)))
    {
        cout << "Number: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << endl;
    }
    file.close();
}

void printReport(string filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line)) cout << line << endl;
    file.close();
}

int main()
{
    string binaryFile, reportFile;
    int numberOfEntries;
    double hourlyPayment;

    cout << "Enter binary file name: ";
    cin >> binaryFile;
    cout << "Enter number of entries: ";
    cin >> numberOfEntries;

    string creatorCmd = "Creator.exe " + binaryFile + " " + to_string(numberOfEntries);

    STARTUPINFO startupInfoCreator;
    PROCESS_INFORMATION processInfoCreator;
    ZeroMemory(&startupInfoCreator, sizeof(startupInfoCreator));
    startupInfoCreator.cb = sizeof(startupInfoCreator);

    CreateProcess(NULL, (LPSTR)creatorCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfoCreator, &processInfoCreator);

    WaitForSingleObject(processInfoCreator.hProcess, INFINITE);
    CloseHandle(processInfoCreator.hProcess);
    CloseHandle(processInfoCreator.hThread);

    printBinaryFile(binaryFile);

    cout << "\nEnter report file name: ";
    cin >> reportFile;
    cout << "Enter payment rate: ";
    cin >> hourlyPayment;

    string reporterCmd = "Reporter.exe " + binaryFile + " " + reportFile + " " + to_string(hourlyPayment);

    STARTUPINFO startInfoReporter;
    PROCESS_INFORMATION processInfoReporter;
    ZeroMemory(&startInfoReporter, sizeof(startInfoReporter));
    startInfoReporter.cb = sizeof(startInfoReporter);

    CreateProcess(NULL, (LPSTR)reporterCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &startInfoReporter, &processInfoReporter);

    WaitForSingleObject(processInfoReporter.hProcess, INFINITE);
    CloseHandle(processInfoReporter.hProcess);
    CloseHandle(processInfoReporter.hThread);

    printReport(reportFile);

    return 0;
}