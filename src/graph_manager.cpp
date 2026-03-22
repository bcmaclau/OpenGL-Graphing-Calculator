#include "graph_manager.h"

//Initalizing all parts of the graphing calculator
GraphManager::GraphManager() {
    //Initializing GLFW and GLAD
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "calc", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create glfwWindow" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    
    glViewport(0, 0, 1280, 720);

    //Initializing struct values for the graph
    graphRect = { 4.5f, -4.5f, -8.0f, 8.0f, 16.0f, 9.0f };
    screenRect = { 720.0f, 0.0f, 1280.0f, 0.0f, 1280.0f, 720.0f };

    //Initializing the shader used 
    solidShader = new Shader("src/sVertexShader.glsl", "src/sFragmentShader.glsl");
    glm::mat4 projection = glm::ortho(graphRect.left, graphRect.right, graphRect.bottom, graphRect.top, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    solidShader->use();
    solidShader->set_mat4(&projection, "projection");
    solidShader->set_mat4(&view, "view");

    //Initializing the Command Manager
    commandManager = new CommandManager(&commandQueue, &commandMutex, &commandCV);
    
    //Initializing the Axes
    axes = new Axes();
    axes->calculate_axes(graphRect);
}

GraphManager::~GraphManager() {
    glfwDestroyWindow(window);
    delete solidShader;
    delete axes;
    glfwTerminate();
}

void GraphManager::run() {
    std::thread commandThread(&CommandManager::command_loop, commandManager);

    while (!glfwWindowShouldClose(window)) {
        process_commands();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    commandThread.join();
}

void GraphManager::process_commands() {
    if (!commandQueue.empty()) {
        if (commandQueue.front() == "quit") {
            glfwSetWindowShouldClose(window, true);
        }
        else if (commandQueue.front() == "add" && commandQueue.size() == 3) {
            commandQueue.pop();
            std::string name = commandQueue.front();
            commandQueue.pop();
            std::string expression = commandQueue.front();
            commandQueue.pop();

            std::cout << name << "(2) = " << MathManager::eval_ExpressionTree(MathManager::RPN_to_ExpressionTree(MathManager::infix_to_RPN(expression)), 2.0f) << std::endl;

            commandCV.notify_one();
        }
    }
}

void GraphManager::render() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    axes->render(solidShader);
}