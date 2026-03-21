#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "graph_manager.h"

int main() {
    GraphManager* graph = new GraphManager();
    
    graph->run();

    return 0;
}
