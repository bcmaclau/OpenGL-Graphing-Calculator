#include <graph.h>

Graph::Graph() {
    //Initializing OpenGL, the rendering window, GLAD, and the solid shader
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "calc", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window :(" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, 1280, 720);

    screenRect = { 0.0f, 1280.0f, 0.0f, 720.0f, 1280.0f, 720.0f };
    graphRect = { -8.0f, 8.0f, -4.5f, 4.5f, 16.0f, 9.0f };

    solidShader = new Shader("src/sVertexShader.glsl", "src/sFragmentShader.glsl");
    glm::mat4 projection = glm::ortho(graphRect.left, graphRect.right, graphRect.bottom, graphRect.top, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    solidShader->use();
    solidShader->set_mat4(&projection, "projection");
    solidShader->set_mat4(&view, "view");

    //Initializing the VAOs and VBOs
    axesArrayBuffer[0] = 0.0f;
    axesArrayBuffer[1] = graphRect.top;
    axesArrayBuffer[2] = 0.0f;
    axesArrayBuffer[3] = graphRect.bottom;
    axesArrayBuffer[4] = graphRect.left;
    axesArrayBuffer[5] = 0.0f;
    axesArrayBuffer[6] = graphRect.right;
    axesArrayBuffer[7] = 0.0f;

    glGenVertexArrays(1, &aVAO);
    glBindVertexArray(aVAO);
    glGenBuffers(1, &aVBO);
    glBindBuffer(GL_ARRAY_BUFFER, aVBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), axesArrayBuffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    graphArrayBuffer = (float*)malloc(0);
    numVertices = 0;
    regenArrayBuffer = false;
    colorCycle = 0;

    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, graphArrayBuffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glLineWidth(2.0f);

    leftButtonDown = false;
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        static_cast<Graph*>(glfwGetWindowUserPointer(w))->mouse_button_callback(w, button, action, mods);
    });

    firstMouse = true;
    lastX = 0.0f;
    lastY = 0.0f;
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<Graph*>(glfwGetWindowUserPointer(w))->mouse_pos_callback(w, xpos, ypos);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Graph*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
    });
}

Graph::~Graph() {
    glfwDestroyWindow(window);
    delete solidShader;
    glfwTerminate();
    free(graphArrayBuffer);
}

void Graph::run() {
    std::thread inputThread(&Graph::input_thread, this);

    while (!glfwWindowShouldClose(window)) {
        process_input();

        if (regenArrayBuffer) {
            gen_graph_array_buffer();
            gen_axes_array();
            regenArrayBuffer = false;
        }

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    inputThread.join();
}
