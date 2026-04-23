#include <graph.h>

Graph::ExpTree* Graph::gen_exp_tree(std::string name) {
    //The first half of this function converts the infix string to a queue of parsed string tokens in RPN using the shunting yard algorithm
    std::string input = nameToInfix[name];
    char token;
    std::string currentDigit = "";
    std::queue<std::string> outputQueue;
    std::stack<std::string> operatorStack;

    std::unordered_map<char, int> precedences;
    std::unordered_map<char, int> associativities;

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

        if (std::isdigit(token) || token == '.' || (token == '-' && currentDigit == "")) {
            currentDigit.append(1, token);
        }
        if (token == 'x') {
            if (currentDigit == "-") {
                currentDigit.append("1");
            }
            if (currentDigit != "") {
                outputQueue.push(currentDigit);
                currentDigit = "";

                token = '*';
                if (!operatorStack.empty()) {
                    std::string o2 = operatorStack.top();
                    while (!operatorStack.empty() && o2 != "(" && (precedences[o2.at(0)] > precedences[token] || (precedences[o2.at(0)] == precedences[token] && associativities[token] == 'l'))) {
                        outputQueue.push(operatorStack.top());
                        operatorStack.pop();
                    }
                }
                operatorStack.push(std::string(1, token));
            }
            currentDigit = "x";
        }
        else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
            outputQueue.push(currentDigit);
            currentDigit = "";
            
            if (!operatorStack.empty()) {
                std::string o2 = operatorStack.top();
                while (!operatorStack.empty() && o2 != "(" && (precedences[o2.at(0)] > precedences[token] || (precedences[o2.at(0)] == precedences[token] && associativities[token] == 'l'))) {
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
            }
            operatorStack.push(std::string(1, token));
            
        }
        else if (token == '(') {
            outputQueue.push(currentDigit);
            currentDigit = "";
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

    //The infix expression is now converted into a queue of tokens in RPN
    //The queue will now be transformed into an expression tree
    //The benefit for calculating values using expression trees vs RPN is there are no string comparisons or conversions
    std::string t;
    std::stack<ExpTree*> treeStack;

    while (!outputQueue.empty()) {
        t = outputQueue.front();
        
        outputQueue.pop();
        char firstC = t.at(0);

        if (firstC == '-' || std::isdigit(firstC) || firstC == '.') {
            ExpTree* newTree = new ExpTree();
            newTree->type = 'c';
            newTree->data = std::stof(t);
            newTree->left = nullptr;
            newTree->right = nullptr;
            treeStack.push(newTree);
        }
        else if (t == "x") {
            ExpTree* newTree = new ExpTree();
            newTree->type = 'x';
            newTree->data = 0.0f;
            newTree->left = nullptr;
            newTree->right = nullptr;
            treeStack.push(newTree);
        }
        else if (t == "+" || t == "-" || t == "*" || t == "/" || t == "^") {
            ExpTree* newTree = new ExpTree();
            newTree->type = firstC;
            newTree->data = 0.0f;
            newTree->right = treeStack.top();
            treeStack.pop();
            newTree->left = treeStack.top();
            treeStack.pop();
            treeStack.push(newTree);
        }
    }

    return treeStack.top();
}

float Graph::eval_exp_tree(ExpTree* tree, float xValue) {
    if (tree->type == 'c') {
        return tree->data;
    }
    else if (tree->type == 'x') {
        return xValue;
    }
    else if (tree->type == '+') {
        return eval_exp_tree(tree->left, xValue) + eval_exp_tree(tree->right, xValue);
    }
    else if (tree->type == '-') {
        return eval_exp_tree(tree->left, xValue) - eval_exp_tree(tree->right, xValue);
    }
    else if (tree->type == '*') {
        return eval_exp_tree(tree->left, xValue) * eval_exp_tree(tree->right, xValue);
    }
    else if (tree->type == '/') {
        return eval_exp_tree(tree->left, xValue) / eval_exp_tree(tree->right, xValue);
    }
    return std::powf(eval_exp_tree(tree->left, xValue), eval_exp_tree(tree->right, xValue));
}

void Graph::gen_graph_array_buffer() {
    //Resize the size pointer holding the data to the correct size
    size_t newBufferSize = 2 * sizeof(float) * (size_t)screenRect.width * nameToExpTree.size();
    numVertices = newBufferSize / (2 * sizeof(float));
    graphArrayBuffer = (float*)realloc(graphArrayBuffer, newBufferSize);

    //Recalculate the data by regenerating the points from their exp_trees
    int arrayIndex = 0;
    float x, y;
    float deltaPixel = graphRect.width / screenRect.width;
    for (const auto& pair : nameToExpTree) {
        for (int i = 0; i < (int)screenRect.width; i++) {
            x = graphRect.left + i * deltaPixel;
            y = eval_exp_tree(pair.second, x);
            graphArrayBuffer[arrayIndex] = x;
            graphArrayBuffer[arrayIndex + 1] = y;
            arrayIndex += 2;
        }
    }

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, newBufferSize, graphArrayBuffer, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
