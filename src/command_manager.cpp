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

        if (input == "test") {
            commandQueue->push(input);
            std::unique_lock<std::mutex> lock(*commandMutex);
            commandCV->wait(lock);
        }
    }
}