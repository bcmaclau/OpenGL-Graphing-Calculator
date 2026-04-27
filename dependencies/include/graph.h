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

    //Project Pipeline: Input Fetching Component -> Input Processing Component -> GUI Processing Component -> Math Component -> Rendering Component

public:
    Graph();
    ~Graph();

    //Initial Run
    void run();

private:
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

    /*
    Input Processing Component
    The input and all other components will be processed in the main thread as OpenGL is not thread-safe and no other components interfere with it.
    The processing thread will only process a command once the first string in the command queue is valid, and the length of the queue matches.
    After processing the command, it will signal to the input thread to accept mroe input.
    */
    void process_input();

    /*
    GUI Processing Component
    This component will record and handle all actions the user can input through the graph itself, not the CLI.
    The GUI features include panning, zooming, and window resizing.
    These features will be implemented through GLFW functions and callbacks.
    */
    bool leftButtonDown;
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    
    bool firstMouse;
    float lastX, lastY;
    void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /*
    Math component
    The math component will handle converting expressions entered by the user.
    First it will transform infix expressions into RPN, then to an expression tree.
    Evaluating points of a line and calculating derivatives is easer for computers to do using expression trees.
    */
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
    void simplify_exp_tree(ExpTree* tree);

    //This is an expensive function that should only be called at most once per frame.
    //Because the IP and GUI components can both determine whether the AB should be regenerated, this bool will help buffer the calls.
    bool regenArrayBuffer;
    void gen_graph_array_buffer();
    void gen_axes_array();

    /*
    Rendering Component
    The rendering component will render all expressions entered by the user to the window.
    Rendering will be implemented through OpenGL and GLFW
    This component will keep track of all functions and their expression trees, the order of rending, and the colors of the lines.
    */
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
    void set_solid_color(glm::vec3 rgb);
    std::unordered_map<std::string, glm::vec3> nameToColor;
    int colorCycle;

    RectF graphRect;
    RectF screenRect;

    unsigned int aVAO, aVBO;
    float axesArrayBuffer[8];

    unsigned int gVAO, gVBO;
    float* graphArrayBuffer;
    int numVertices;
    std::vector<std::string> renderOrder;
    
    void render();
};
