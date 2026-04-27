#include <graph.h>

Graph::ExpTree* Graph::gen_exp_tree(std::string name) {
    std::string infix = nameToInfix[name];
    std::stack<char> operatorStack;
    std::stack<ExpTree*> nodeStack;

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

    std::string operand;

    std::string sugar = "";
    for (int i = 0; i < infix.size(); i++) {
        char token = infix.at(i);
        sugar += token;
        if ((std::isdigit(token) && i + 1 < infix.size() && (infix.at(i + 1) == '(' || infix.at(i + 1) == 'x')) ||
            (token == 'x' && i + 1 < infix.size() && infix.at(i + 1) == '(') ||
            (token == ')' && i + 1 < infix.size() && (std::isdigit(infix.at(i + 1)) || infix.at(i + 1) == 'x')) ||
            (token == ')' && i + 1 < infix.size() && infix.at(i + 1) == '(')) {
            sugar += '*';
        }
        else if ((token == '-' && i == 0 && infix.at(i + 1) == '(')) {
            sugar += '1';
            sugar += '*';
        }
    }
    infix = sugar;

    for (int i = 0; i < infix.size(); i++) {
        char token = infix.at(i);

        if (std::isdigit(token) || token == '.' || (i == 0 && token == '-')) {
            operand = "";
            operand += token;
            for (int j = i + 1; j < infix.size() && (std::isdigit(infix.at(j)) || infix.at(j) == '.'); j++) {
                operand += infix.at(j);
                i++;
            }
            ExpTree* cNode = new ExpTree();
            cNode->type = 'c';
            cNode->data = std::stof(operand);
            cNode->left = nullptr;
            cNode->right = nullptr;
            nodeStack.push(cNode);
        }
        else if (token == 'x') {
            ExpTree* xNode = new ExpTree();
            xNode->type = 'x';
            xNode->data = 0.0f;
            xNode->left = nullptr;
            xNode->right = nullptr;
            nodeStack.push(xNode);
        }
        else if (token == '(') {
            operatorStack.push('(');
        }
        else if (token == ')') {
            char op = operatorStack.top();
            operatorStack.pop();
            while (op != '(') {
                ExpTree* opNode = new ExpTree();
                opNode->type = op;
                opNode->data = 0.0f;
                opNode->right = nodeStack.top();
                nodeStack.pop();
                opNode->left = nodeStack.top();
                nodeStack.pop();
                nodeStack.push(opNode);

                op = operatorStack.top();
                operatorStack.pop();
            }
        }
        else if (precedences.find(token) != precedences.end()) {
            while (!operatorStack.empty()) {
                char top = operatorStack.top();

                if (top != '(' && (precedences[top] > precedences[token] || (precedences[top] == precedences[token] && associativities[token] == 'l'))) {
                    operatorStack.pop();
                    
                    ExpTree* opNode = new ExpTree();
                    opNode->type = top;
                    opNode->data = 0.0f;
                    opNode->right = nodeStack.top();
                    nodeStack.pop();
                    opNode->left = nodeStack.top();
                    nodeStack.pop();
                    nodeStack.push(opNode);
                }
                else {
                    break;
                }
            }

            operatorStack.push(token);
        }
    }

    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();
        ExpTree* opNode = new ExpTree();
        opNode->type = op;
        opNode->data = 0.0f;
        opNode->right = nodeStack.top();
        nodeStack.pop();
        opNode->left = nodeStack.top();
        nodeStack.pop();
        nodeStack.push(opNode);
    }

    return nodeStack.top();
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
    renderOrder.clear();
    for (const auto& pair : nameToExpTree) {
        renderOrder.push_back(pair.first);
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
