#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <cmath>

#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Graph {
public:
    Graph();
    ~Graph();

    /*
    Project Pipeline: Input Fetching Component -> Input Processing Component -> Math Component -> Rendering Component
    */

    /*
    Input Fetching Component
    The input function will push command strings to a queue.
    The input will be handled in its own thread because OpenGl will pause when reading user input in the same thread.
    Once a full command has been entered, the thread will wait until the last command is processed before accepting more input.
    To steer very clear from race conditions, the input thread will only post to the command queue and only the input processing component will interact with the command queue by polling it.
    A condition variable will be used to notify the input thread once a command has been processed to avoid busy waiting.
    */
    std::queue<std::string> commandQueue;
    std::mutex inputMutex;
    std::condition_variable inputCV;
    void input_thread();

    //Input Processing Component
    //The input and all other components will be processed in the main thread as OpenGL is not thread-safe and no other components interfere with it.
    //The processing thread will only process a command once the first string in the command queue is valid, and the length of the queue matches.
    //After processing the command, it will signal to the input thread to accept mroe input.
    void process_input();

    //Math component
    //The math component will handle converting expressions entered by the user.
    //First it will transform infix expressions into RPN, then to an expression tree.
    //Evaluating points of a line and calculating derivatives is easer for computers to do using expression trees.
    struct ExpTree {
        ExpTree* left;
        ExpTree* right;
        float data;
        char type;
    };

    std::unordered_set<std::string> nameSet;
    std::unordered_map<std::string, std::string> nameToInfix;
    std::unordered_map<std::string, ExpTree*> nameToExpTree;
    
    ExpTree* gen_exp_tree(std::string name);
    float eval_exp_tree(ExpTree* tree, float xValue);
    void gen_graph_array_buffer();

    //Rendering Component
    //The rendering component will render all expression entered by the user to the window.
    struct RectF {
        float left;
        float right;
        float bottom;
        float top;
        float width;
        float height;
    };
    GLFWwindow* window;
    
    Shader* solidShader;
    void set_solid_color(float r, float g, float b);

    RectF graphRect;
    RectF screenRect;

    unsigned int aVAO, aVBO;
    float axesArrayBuffer[8];

    unsigned int gVAO, gVBO;
    float* graphArrayBuffer;
    int numVertices;
    
    void render();

    //Initial Run
    void run();
};
