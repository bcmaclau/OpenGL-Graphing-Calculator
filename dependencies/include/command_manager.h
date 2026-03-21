#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class CommandManager {
public:
    CommandManager(std::queue<std::string>* cq, std::mutex* cm, std::condition_variable* ccv);

    void command_loop();

private:
    std::queue<std::string>* commandQueue;
    std::mutex* commandMutex;
    std::condition_variable* commandCV;
};