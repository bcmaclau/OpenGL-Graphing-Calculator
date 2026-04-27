#include <graph.h>

void Graph::input_thread() {
    std::string input = "";
    while (input != "quit") {
        std::cout << "Enter command: ";
        std::cin >> input;

        if (input == "quit") {
            commandQueue.push(input);
        }

        else if (input == "add") {
            commandQueue.push(input);
            
            //Code to enter a valid name
            //Requirements: 1 char long, isAlpha, and unique
            std::cout << "Enter function name: ";
            std::cin >> input;
            bool validLength = false, validChar = false, unique = false;
            while (!validLength || !validChar || !unique) {
                if (input.size() > 1) {
                    while (input.size() > 1) {
                        std::cout << "Invalid name, names must be 1 character long. Reenter: ";
                        std::cin >> input;
                    }
                }
                else {
                    validLength = true;
                }
                if (!std::isalpha(input.at(0))) {
                    while (!std::isalpha(input.at(0))) {
                        std::cout << "Invalid name, names must contain only letters. Reenter: ";
                        std::cin >> input;
                    }
                }
                else {
                    validChar = true;
                }
                if (nameSet.find(input) != nameSet.end()) {
                    while (nameSet.find(input) != nameSet.end()) {
                        std::cout << "Invalid name, you can not have more than one funciton with the same name. Reenter: ";
                        std::cin >> input;
                    }
                }
                else {
                    unique = true;
                }
            }
            commandQueue.push(input);

            std::cout << "Enter expression: ";
            std::cin >> input;
            commandQueue.push(input);

            std::unique_lock<std::mutex> lock(inputMutex);
            inputCV.wait(lock);
        }

        else if (input == "remove") {
            if (nameSet.size() == 0) {
                std::cout << "There are no functions to remove." << std::endl;
            }
            else {
                commandQueue.push(input);

                std::cout << "Enter function to remove: ";
                std::cin >> input;
                while (nameSet.find(input) == nameSet.end()) {
                    std::cout << "No function named \"" << input << "\", Reenter: ";
                    std::cin >> input;
                }

                commandQueue.push(input);

                std::unique_lock<std::mutex> lock(inputMutex);
                inputCV.wait(lock);
            }
        }

        else if (input == "list") {
            commandQueue.push(input);

            std::unique_lock<std::mutex> lock(inputMutex);
            inputCV.wait(lock);
        }
    }
}
