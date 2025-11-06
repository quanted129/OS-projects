#include "header.h"

int main(int argc, char* argv[])
{
    std::string fileName = argv[1];
    MessageQueue queue;

    queue.Open(fileName);
    queue.SignalReady();

    std::string command;
    bool run = 1;
    while (run)
    {
        std::cout << "Enter command (send/exit): ";
        std::cin >> command;

        if (command == "send")
        {
            std::string message;
            std::cout << "Enter message: ";
            std::cin.ignore();
            std::getline(std::cin, message);

            if (queue.SendMessage(message))
            {
                std::cout << "Message sent: " << message << std::endl;
            }
        }
        else if (command == "exit")
        {
            run = 0;
        }
    }
    return 0;
}