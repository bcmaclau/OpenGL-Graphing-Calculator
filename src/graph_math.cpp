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
    precedences['s'] = 5;
    associativities['s'] = 'r';
    precedences['c'] = 5;
    associativities['c'] = 'r';
    precedences['t'] = 5;
    associativities['t'] = 'r';

    std::string operand;

    std::string sugar = "";
    for (int i = 0; i < infix.size(); i++) {
        char token = infix.at(i);
        sugar += token;
        if ((std::isdigit(token) && i + 1 < infix.size() && (infix.at(i + 1) == '(' || infix.at(i + 1) == 'x' || infix.at(i + 1) == 'p' || infix.at(i + 1) == 'e' || infix.at(i + 1) == 's' || infix.at(i + 1) == 'c')) ||
            (token == 'x' && i + 1 < infix.size() && (infix.at(i + 1) == '(' || infix.at(i + 1) == 'x' || infix.at(i + 1) == 'p' || infix.at(i + 1) == 'e' || infix.at(i + 1) == 's' || infix.at(i + 1) == 'c')) ||
            (token == ')' && i + 1 < infix.size() && (std::isdigit(infix.at(i + 1)) || infix.at(i + 1) == 'x' || infix.at(i + 1) == '(' || infix.at(i + 1) == 'p' || infix.at(i + 1) == 'e' || infix.at(i + 1) == 's' || infix.at(i + 1) == 'c')) ||
            (token == 'e' && i + 1 < infix.size() && (infix.at(i + 1) == 'x' || infix.at(i + 1) == '(' || std::isdigit(infix.at(i + 1)) || infix.at(i + 1) == 'p' || infix.at(i + 1) == 'e' || infix.at(i + 1) == 's' || infix.at(i + 1) == 'c'))) {
            sugar += '*';
        }
        else if ((token == '-' && i == 0 && infix.at(i + 1) == '(')) {
            sugar += '1';
            sugar += '*';
        }
        else if ((token == 'c' && infix.at(i + 1) == 'o' && infix.at(i + 2) == 's') || 
            (token == 's' && infix.at(i + 1) == 'i' && infix.at(i + 2) == 'n') || 
            (token == 't' && infix.at(i + 1) == 'a' && infix.at(i + 2) == 'n')) {
            i += 2;
        }
        else if (token == 'p' && infix.at(i + 1) == 'i') {
            i++;
            if (i + 1 < infix.size() && (infix.at(i + 1) == 'x' || infix.at(i + 1) == '(' || std::isdigit(infix.at(i + 1)) || infix.at(i + 1) == 'p' || infix.at(i + 1) == 'e' || infix.at(i + 1) == 's' || infix.at(i + 1) == 'c')) {
                sugar += '*';
            }
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
            cNode->type = '1';
            cNode->data = std::stof(operand);
            cNode->left = nullptr;
            cNode->right = nullptr;
            nodeStack.push(cNode);
        }
        else if (token == 'x' || token == 'e' || token == 'p') {
            ExpTree* xNode = new ExpTree();
            xNode->type = token;
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

        if (op == 's' || op == 'c' || op == 't') {
            ExpTree* opNode = new ExpTree();
            opNode->type = op;
            opNode->data = 0.0f;
            opNode->right = nodeStack.top();
            nodeStack.pop();
            opNode->left = nullptr;
            nodeStack.push(opNode);
        }
        else {
            ExpTree* opNode = new ExpTree();
            opNode->type = op;
            opNode->data = 0.0f;
            opNode->right = nodeStack.top();
            nodeStack.pop();
            opNode->left = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(opNode);
        }
    }

    return nodeStack.top();
}

float Graph::eval_exp_tree(ExpTree* tree, float xValue) {
    float r;
    switch (tree->type) {
        case '1':
            r = tree->data;
            break;
        case 'x':
            r = xValue;
            break;
        case 'e':
            r = std::expf(1.0f);
            break;
        case 'p':
            r = M_PI;
            break;
        case '+':
            r = eval_exp_tree(tree->left, xValue) + eval_exp_tree(tree->right, xValue);
            break;
        case '-':
            r = eval_exp_tree(tree->left, xValue) - eval_exp_tree(tree->right, xValue);
            break;
        case '*':
            r = eval_exp_tree(tree->left, xValue) * eval_exp_tree(tree->right, xValue);
            break;
        case '/':
            r = eval_exp_tree(tree->left, xValue) / eval_exp_tree(tree->right, xValue);
            break;
        case '^':
            r = std::powf(eval_exp_tree(tree->left, xValue), eval_exp_tree(tree->right, xValue));
            break;
        case 's':
            r = std::sinf(eval_exp_tree(tree->right, xValue));
            break;
        case 'c':
            r = std::cosf(eval_exp_tree(tree->right, xValue));
            break;
        case 't':
            r = std::tanf(eval_exp_tree(tree->right, xValue));
            break;
        default:
            r = -1.0f;
            break;
    }
    return r;
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

    glm::mat4 projection = glm::ortho(graphRect.left, graphRect.right, graphRect.bottom, graphRect.top, 0.1f, 100.0f);
    solidShader->use();
    solidShader->set_mat4(&projection, "projection");

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, newBufferSize, graphArrayBuffer, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Graph::gen_axes_array() {
    axesArrayBuffer[0] = 0.0f;
    axesArrayBuffer[1] = graphRect.top;
    axesArrayBuffer[2] = 0.0f;
    axesArrayBuffer[3] = graphRect.bottom;
    axesArrayBuffer[4] = graphRect.left;
    axesArrayBuffer[5] = 0.0f;
    axesArrayBuffer[6] = graphRect.right;
    axesArrayBuffer[7] = 0.0f;

    glBindVertexArray(aVAO);
    glBindBuffer(GL_ARRAY_BUFFER, aVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), axesArrayBuffer);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
