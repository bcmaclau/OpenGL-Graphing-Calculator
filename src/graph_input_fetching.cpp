#include <graph.h>

void Graph::input_thread() {
    std::string input = "";
    while (input != "quit") {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            commandQueue.push(input);
        }

        else if (input == "add") {
            commandQueue.push(input);
            
            //Code to enter a valid name
            //Requirements: 1 char long, isAlpha, and unique
            std::cout << "Enter function name: ";
            std::getline(std::cin, input);
            bool validLength = false, validChar = false, unique = false;
            while (!validLength || !validChar || !unique) {
                if (input.size() > 1) {
                    while (input.size() > 1) {
                        std::cout << "Invalid name, names must be 1 character long. Reenter: ";
                        std::getline(std::cin, input);
                    }
                }
                else {
                    validLength = true;
                }
                if (!std::isalpha(input.at(0))) {
                    while (!std::isalpha(input.at(0))) {
                        std::cout << "Invalid name, names must contain only letters. Reenter: ";
                        std::getline(std::cin, input);
                    }
                }
                else {
                    validChar = true;
                }
                if (nameSet.find(input) != nameSet.end()) {
                    while (nameSet.find(input) != nameSet.end()) {
                        std::cout << "Invalid name, you can not have more than one funciton with the same name. Reenter: ";
                        std::getline(std::cin, input);
                    }
                }
                else {
                    unique = true;
                }
            }
            commandQueue.push(input);

            std::cout << "Enter expression: ";
            std::getline(std::cin, input);
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
                std::getline(std::cin, input);
                while (nameSet.find(input) == nameSet.end()) {
                    std::cout << "No function named \"" << input << "\", Reenter: ";
                    std::getline(std::cin, input);
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

        else if (input == "eval") {
            if (nameSet.size() == 0) {
                std::cout << "There are no functions to evaluate." << std::endl;
            }
            else {
                commandQueue.push(input);

                std::cout << "Enter function to evaluate: ";
                std::getline(std::cin, input);
                while (nameSet.find(input) == nameSet.end()) {
                    std::cout << "No function named \"" << input << "\", Reenter: ";
                    std::getline(std::cin, input);
                }
                commandQueue.push(input);

                std::cout << "Enter value: ";
                std::getline(std::cin, input);
                bool valid = false;
                while (!valid) {
                    valid = true;
                    bool hasDot = false;
                    for (int i = 0; i < input.size(); i++) {
                        if (std::isdigit(input.at(i)) || input.at(i) == '.' || (i == 0 && input.at(0) == '-' && input.size() >= 2 && std::isdigit(input.at(1)))) {
                            if (input.at(i) == '.') {
                                if (hasDot) {
                                    valid = false;
                                }
                                else {
                                    hasDot = true;
                                }
                            }
                        }
                        else {
                            valid = false;
                        }
                    }
                    if (!valid) {
                        std::cout << "Invalid value \"" << input << "\", Reenter: ";
                        std::getline(std::cin, input);
                    }
                }
                commandQueue.push(input);

                std::unique_lock<std::mutex> lock(inputMutex);
                inputCV.wait(lock);
            }
        }

        else if (nameSet.find(std::string(1, input.at(0))) != nameSet.end()) {
            if (input.size() >= 4 && input.at(1) == '(' && input.at(input.size() - 1) == ')') {
                std::string xValue = "";
                bool valid = true;
                bool hasDot = false;
                for (int i = 2; i < input.size() - 1 && valid; i++) {
                    if (std::isdigit(input.at(i)) || input.at(i) == '.' || (i == 2 && input.at(2) == '-' && std::isdigit(input.at(3)))) {
                        if (input.at(i) == '.') {
                            if (hasDot) {
                                valid = false;
                            }
                            else {
                                hasDot = true;
                            }
                        }
                        xValue += input.at(i);
                    }
                    else {
                        valid = false;
                    }
                }
                if (valid) {
                    commandQueue.push("eval");
                    commandQueue.push(std::string(1, input.at(0)));
                    commandQueue.push(xValue);

                    std::unique_lock<std::mutex> lock(inputMutex);
                    inputCV.wait(lock);
                }
                else {
                    std::cout << "Invalid Command. Type \"help\" to list all commands." << std::endl;
                }
            }
            else {
                std::cout << "Invalid Command. Type \"help\" to list all commands." << std::endl;
            }
        }

        else {
            std::string noSpaces = "";
            for (int i = 0; i < input.size(); i++) {
                if (input.at(i) != ' ') {
                    noSpaces += input.at(i);
                }
            }
            input = noSpaces;

            if (input.size() >= 6 && std::isalpha(input.at(0)) && nameSet.find(std::string(1, input.at(0))) == nameSet.end() &&
                        input.at(1) == '(' && input.at(2) == 'x' && input.at(3) == ')' && input.at(4) == '=') {

                std::string expression = "";
                for (int i = 5; i < input.size(); i++) {
                    expression += input.at(i);
                }

                //TODO: Make a function to check if an expression is valid
                commandQueue.push("add");
                commandQueue.push(std::string(1, input.at(0)));
                commandQueue.push(expression);

                std::unique_lock<std::mutex> lock(inputMutex);
                inputCV.wait(lock);
            }
            else {
                std::cout << "Invalid Command. Type \"help\" to list all commands." << std::endl;
            }
        }
    }
}
