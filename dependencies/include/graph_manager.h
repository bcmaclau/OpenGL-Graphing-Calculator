#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "command_manager.h"
#include "axes.h"
#include "math_manager.h"
#include "shader.h"
#include "rectf.h"

class GraphManager {
public:
    GraphManager();
    ~GraphManager();

    void run();

private:
    GLFWwindow* window;

    Shader* solidShader;

    Rectf graphRect, screenRect;

    CommandManager* commandManager;
    std::queue<std::string> commandQueue;
    std::mutex commandMutex;
    std::condition_variable commandCV;

    Axes* axes;

    void process_commands();
    void render();
};
