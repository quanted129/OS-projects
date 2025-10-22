#include <fstream>
#include <string>
#include "../Main/header.h"
using namespace std;

int main(int argc, char* argv[])
{
    char* inputFile = argv[1];
    char* outputFile = argv[2];
    double rate = stod(argv[3]);

    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile);
    out << "Reporting " << inputFile << ":\n";

    employee emp;
    while (in.read((char*)&emp, sizeof(employee)))
    {
        double salary = emp.hours * rate;
        out << "Num: " << emp.num << ", name: " << emp.name << ", hours: " << emp.hours << ", salary: " << salary << '\n';
    }

    in.close();
    out.close();
    return 0;
}