#include <graph.h>

void Graph::process_input() {
    if (!commandQueue.empty()) {
        if (commandQueue.front() == "quit") {
            glfwSetWindowShouldClose(window, true);
        }

        else if (commandQueue.front() == "add" && commandQueue.size() == 3) {

            commandQueue.pop();
            std::string name = commandQueue.front();
            commandQueue.pop();
            std::string infix = commandQueue.front();
            commandQueue.pop();

            nameSet.insert(name);
            nameToInfix[name] = infix;
            nameToFInfix[name] = format_infix_expression(infix);
            nameToExpTree[name] = gen_exp_tree(name);

            regenArrayBuffer = true;

            switch (colorCycle) {
                case 0:
                    nameToColor[name] = glm::vec3(1.0f, 0.0f, 0.0f);
                    colorCycle++;
                    break;
                case 1:
                    nameToColor[name] = glm::vec3(0.0f, 0.0f, 1.0f);
                    colorCycle++;
                    break;
                case 2:
                    nameToColor[name] = glm::vec3(0.0f, 1.0f, 0.0f);
                    colorCycle++;
                    break;
                case 3:
                    nameToColor[name] = glm::vec3(1.0f, 0.5f, 0.0f);
                    colorCycle++;
                    break;
                case 4:
                    nameToColor[name] = glm::vec3(0.5f, 0.0f, 0.5f);
                    colorCycle++;
                    break;
                default:
                    nameToColor[name] = glm::vec3(0.0f, 0.0f, 0.0f);
                    colorCycle = 0;
                    break;
            }

            inputCV.notify_one();
        }

        else if (commandQueue.front() == "remove" && commandQueue.size() == 2) {
            commandQueue.pop();

            std::string name = commandQueue.front();
            commandQueue.pop();

            nameSet.erase(name);
            nameToInfix.erase(name);
            nameToColor.erase(name);
            nameToExpTree.erase(name);

            regenArrayBuffer = true;

            inputCV.notify_one();
        }

        else if (commandQueue.front() == "list") {
            commandQueue.pop();

            if (nameSet.size() == 0) {
                std::cout << "No functions entered yet." << std::endl;
            }
            else {
                std::cout << "List of all functions:" << std::endl;
                for (const auto& pair : nameToInfix) {
                    std::cout << pair.first << "(x) = " << pair.second << std::endl;
                }
            }

            inputCV.notify_one();
        }

        else if (commandQueue.front() == "eval" && commandQueue.size() == 3) {
            commandQueue.pop();

            std::string name = commandQueue.front();
            commandQueue.pop();
            float xValue = std::stof(commandQueue.front());
            commandQueue.pop();

            std::cout << name << "(" << xValue << ") = " << eval_exp_tree(nameToExpTree[name], xValue) << std::endl;

            inputCV.notify_one();
        }
    }
}

std::string Graph::format_infix_expression(std::string exp) {
    std::string noSugar = "";
    
    std::unordered_set<char> multSet;
    multSet.insert('x');
    multSet.insert('e');
    multSet.insert('p');
    multSet.insert('s');
    multSet.insert('c');
    multSet.insert('t');
    multSet.insert('(');

    std::unordered_set<char> funcSet;
    funcSet.insert('s');
    funcSet.insert('c');
    funcSet.insert('t');

    for (int i = 0; i < exp.size(); i++) {
        char token = exp.at(i);
        noSugar += token;
        
        //Adding multiplication between float values, numeric constants, functions, and parenthesis
        if (exp.size() > i + 1) {
            if ((std::isdigit(token) && multSet.find(exp.at(i + 1)) != multSet.end()) ||
                (token != '(' && multSet.find(token) != multSet.end() && multSet.find(exp.at(i + 1)) != multSet.end()) ||
                (token == ')' && multSet.find(exp.at(i + 1)) != multSet.end())) {
                noSugar += '*';
            }
            else if (token == '-') {
                if (i == 0) {
                    noSugar.at(0) = '0';
                    noSugar += '-';
                }
                else if (exp.at(i - 1) == '(') {
                    noSugar.at(noSugar.size() - 1) = '0';
                    noSugar += '-';
                }
            }
            else if (token == 's' || token == 'c' || token == 't') {
                i += 2;
            }
            else if (token == 'p') {
                i++;
            }

            if (funcSet.find(token) != funcSet.end() && noSugar.at(noSugar.size() - 1) == '*') {
                noSugar.erase(noSugar.size() - 1);
            }
        }
    }

    return noSugar;
}

bool Graph::valid_expression(std::string exp) {
    exp = format_infix_expression(exp);

    if (exp.empty()) { return false; }

    std::unordered_set<char> operatorSet;
    operatorSet.insert('+');
    operatorSet.insert('-');
    operatorSet.insert('*');
    operatorSet.insert('/');
    operatorSet.insert('^');

    std::unordered_set<char> operandSet;
    operandSet.insert('x');
    operandSet.insert('e');
    operandSet.insert('p');
    operandSet.insert('(');
    operandSet.insert('.');
    operandSet.insert('s');
    operandSet.insert('c');
    operandSet.insert('t');

    std::unordered_set<char> validEnding;
    validEnding.insert('x');
    validEnding.insert(')');
    validEnding.insert('e');
    validEnding.insert('p');
    validEnding.insert('.');

    if (exp.size() == 1) { return (std::isdigit(exp.at(0)) || validEnding.find(exp.at(0)) != validEnding.end()) && exp.at(exp.size() - 1) != ')'; }
    if (operatorSet.find(exp.at(0)) != operatorSet.end()) { return false; }

    bool containsDot = false;

    int parenCount = 0;

    for (int i = 0; i < exp.size() - 1; i++) {
        char token = exp.at(i);
        char next = exp.at(i + 1);
        
        if (std::isdigit(token)) {
            if (next == '.') {
                if (containsDot) {
                    return false;
                }
                else {
                    containsDot = true;
                }
            }
            else if (!std::isdigit(next) && operatorSet.find(next) == operatorSet.end() && next != ')') {
                return false;
            }
        }
        else if (token == '.') {
            if (next == '.') {
                return false;
            }
            else if (!std::isdigit(next) && operatorSet.find(next) == operatorSet.end() && next != ')') {
                return false;
            }
        }
        else if (token == '(') {
            parenCount++;
            if (next == ')' || (operandSet.find(next) == operandSet.end() && !std::isdigit(next))) {
                return false;
            }
        }
        else if (token == ')') {
            containsDot = false;
            parenCount--;
            if (parenCount < 0) {
                return false;
            }
            if (operatorSet.find(next) == operatorSet.end() && next != ')') {
                return false;
            }
        }
        else if (operandSet.find(token) != operandSet.end()) {
            if (token == 's' || token == 'c' || token == 't') {
                if (operandSet.find(next) == operandSet.end()) {
                    return false;
                }
            }
            else if (token == '(') {
                if (operandSet.find(next) == operandSet.end()) {
                    return false;
                }
            }
            else {
                if (operatorSet.find(next) == operatorSet.end() && next != ')') {
                    return false;
                }
            }
        }
        else if (operatorSet.find(token) != operatorSet.end()) {
            containsDot = false;
            if (operandSet.find(next) == operandSet.end() && !std::isdigit(next)) {
                return false;
            }
        }
        else {
            std::cout << "Invalid Character: " << token << std::endl;
            return false;
        }
    }

    if (exp.at(exp.size() - 1) == ')') {
        parenCount--;
    }

    if (parenCount != 0) {
        return false;
    }

    return std::isdigit(exp.at(exp.size() - 1)) || validEnding.find(exp.at(exp.size() - 1)) != validEnding.end();
}
