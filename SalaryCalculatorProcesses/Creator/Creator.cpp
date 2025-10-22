#include <iostream>
#include <fstream>
#include <string>
#include "../Main/header.h"
using namespace std;

int main(int argc, char* argv[])
{
    char* outputFile = argv[1];
    int recordsCount = stoi(argv[2]);

    ofstream out(outputFile, ios::binary);

    for (int i = 0; i < recordsCount; i++)
    {
        employee emp;
        cout << "Enter employee number: ";
        cin >> emp.num;
        cout << "Enter employee name (max 9 chars): ";
        cin >> emp.name;
        cout << "Enter hours worked: ";
        cin >> emp.hours;

        out.write((char*)&emp, sizeof(employee));
    }

    out.close();
    return 0;
}