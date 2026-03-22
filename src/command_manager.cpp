#include "command_manager.h"

CommandManager::CommandManager(std::queue<std::string>* cq, std::mutex* cm, std::condition_variable* ccv) {
    commandQueue = cq;
    commandMutex = cm;
    commandCV = ccv;
}

void CommandManager::command_loop() {
    std::string input = "";
    while (input != "quit") {
        std::cout << "Enter command: ";
        std::cin >> input;

        if (input == "quit") {
            commandQueue->push(input);
        }
        
        else if (input == "add") {
            commandQueue->push(input);
            
            std::cout << "Enter function name: ";
            std::cin >> input;
            bool validLength = false, validChar = false;
            while (!validLength || !validChar) {
                if (input.size() > 1) {
                    while (input.size() > 1) {
                        std::cout << "Invalid name, names must be 1 character long. reenter: ";
                        std::cin >> input;
                    }
                }
                else {
                    validLength = true;
                }
                if (!std::isalpha(input.at(0))) {
                    while (!std::isalpha(input.at(0))) {
                        std::cout << "Invalid name, names must contain only letters. reenter: ";
                        std::cin >> input;
                    }
                }
                else {
                    validChar = true;
                }
            }

            commandQueue->push(input);
            std::cout << "Enter the expression " << input << "(x) = ";
            std::cin >> input;
            commandQueue->push(input);
            std::unique_lock<std::mutex> lock(*commandMutex);
            commandCV->wait(lock);
        }
    }
}