#pragma once
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\EmployeePipe"
#define CMD_CONNECT 1
#define CMD_READ 2
#define CMD_MODIFY 3
#define CMD_SAVE 4
#define CMD_FINISH 5
#define CMD_EXIT 6

struct employee
{
    int num;
    char name[10];
    double hours;
};

struct Message
{
    int command;
    struct employee data;
    int record_id;
    bool success;
    char error_msg[50];
};