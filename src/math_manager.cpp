#include <math_manager.h>

std::queue<std::string> MathManager::infix_to_RPN(std::string input) {
    char token;
    std::string currentDigit = "";
    std::queue<std::string> outputQueue;
    std::stack<std::string> operatorStack;

    std::unordered_map<char, int> precedences;
    std::unordered_map<char, char> associativities;

    precedences['+'] = 2;
    associativities['+'] = 'l';
    precedences['-'] = 2;
    associativities['-'] = 'l';
    precedences['*'] = 3;
    associativities['*'] = 'l';
    precedences['/'] = 3;
    associativities['/'] = 'l';
    precedences['^'] = 4;
    associativities['^'] = 'r';

    for (int i = 0; i < input.size(); i++) {
        token = input.at(i);

        if (std::isdigit(token) || token == '.') {
            currentDigit += token;
        }
        else if (token == 'x') {
            if (currentDigit != "") {
                if (currentDigit == "-") {
                    currentDigit += '1';
                }
                outputQueue.push(currentDigit);
                outputQueue.push(std::string(1, token));
                currentDigit = "";
                token = '*';

                while ((!operatorStack.empty() && operatorStack.top() != "(") && 
                        (precedences[operatorStack.top().at(0)] > precedences[token] ||
                        (precedences[operatorStack.top().at(0)] == precedences[token] && associativities[token] == 'l'))) {
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(std::string(1, token));
            }
            else {
                outputQueue.push(std::string(1, token));
            }
        }
        else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
            if (token == '-' && currentDigit == "") {
                currentDigit += token;
            }
            else {
                if (currentDigit != "") {
                    outputQueue.push(currentDigit);
                    currentDigit = "";
                }

                while ((!operatorStack.empty() && operatorStack.top() != "(") && 
                        (precedences[operatorStack.top().at(0)] > precedences[token] ||
                        (precedences[operatorStack.top().at(0)] == precedences[token] && associativities[token] == 'l'))) {
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(std::string(1, token));
            }
        }
        else if (token == '(') {
            operatorStack.push(std::string(1, token));
        }
        else if (token == ')') {
            while (operatorStack.top() != "(") {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.pop();
        }
    }

    if (currentDigit != "") {
        outputQueue.push(currentDigit);
    }

    while (!operatorStack.empty()) {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}

MathManager::ExpressionTree* MathManager::RPN_to_ExpressionTree(std::queue<std::string> RPN) {
    std::string token;
    std::stack<ExpressionTree*> nodeStack;

    while (!RPN.empty()) {
        token = RPN.front();
        RPN.pop();
        char firstC = token.at(0);

        if (firstC == '-' || std::isdigit(firstC) || firstC == '.') {
            ExpressionTree* newNode = new ExpressionTree();
            newNode->type = 'c';
            newNode->data = std::stof(token);
            newNode->left = nullptr;
            newNode->right = nullptr;
            nodeStack.push(newNode);
        }
        else if (token == "x") {
            ExpressionTree* newNode = new ExpressionTree();
            newNode->type = 'x';
            newNode->data = 0.0f;
            newNode->left = nullptr;
            newNode->right = nullptr;
            nodeStack.push(newNode);
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            ExpressionTree* newNode = new ExpressionTree();
            newNode->type = firstC;
            newNode->data = 0.0f;
            newNode->right = nodeStack.top();
            nodeStack.pop();
            newNode->left = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(newNode);
        }
    }

    return nodeStack.top();
}

float MathManager::eval_ExpressionTree(ExpressionTree* tree, float x) {
    if (tree->type == 'c') {
        return tree->data;
    }
    else if (tree->type == 'x') {
        return x;
    }
    else if (tree->type == '+') {
        return eval_ExpressionTree(tree->left, x) + eval_ExpressionTree(tree->right, x);
    }
    else if (tree->type == '-') {
        return eval_ExpressionTree(tree->left, x) - eval_ExpressionTree(tree->right, x);
    }
    else if (tree->type == '*') {
        return eval_ExpressionTree(tree->left, x) * eval_ExpressionTree(tree->right, x);
    }
    else if (tree->type == '/') {
        return eval_ExpressionTree(tree->left, x) / eval_ExpressionTree(tree->right, x);
    }
    return std::powf(eval_ExpressionTree(tree->left, x), eval_ExpressionTree(tree->right, x));
}