#pragma once

#include <string>
#include <cctype>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iostream>
#include <cmath>

class MathManager {
public:
    struct ExpressionTree { 
        ExpressionTree* left;
        ExpressionTree* right;
        float data;
        char type;
    };

    static std::queue<std::string> infix_to_RPN(std::string input);
    static ExpressionTree* RPN_to_ExpressionTree(std::queue<std::string> RPN);
    static float eval_ExpressionTree(ExpressionTree* tree, float x);

    static void print_RPN(std::queue<std::string> RPN) {
        while (!RPN.empty()) {
            std::cout << RPN.front() << std::endl;
            RPN.pop();
        }
    } 
};