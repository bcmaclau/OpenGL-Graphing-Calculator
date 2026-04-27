#include "graph.h"

void Graph::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        graphRect.left = -graphRect.width / 2;
        graphRect.right = graphRect.width / 2;
        graphRect.bottom = -graphRect.height / 2;
        graphRect.top = graphRect.height / 2;
        regenArrayBuffer = true;
    }

    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        float xEdgeLoss = (graphRect.width - (graphRect.width * 0.9f)) / 2.0f;
        float yEdgeLoss = (graphRect.height - (graphRect.height * 0.9)) / 2.0f;
        graphRect.width *= 0.9f;
        graphRect.height *= 0.9f;
        graphRect.left += xEdgeLoss;
        graphRect.right -= xEdgeLoss;
        graphRect.bottom += yEdgeLoss;
        graphRect.top -= yEdgeLoss;
        regenArrayBuffer = true;
    }

    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        float xEdgeGain = ((graphRect.width *= 1.1f) - graphRect.width) / 2.0f;
        float yEdgeGain = ((graphRect.height *= 1.1f) - graphRect.height) / 2.0f;
        graphRect.width *= 1.1f;
        graphRect.height *= 1.1f;
        graphRect.left -= xEdgeGain;
        graphRect.right += xEdgeGain;
        graphRect.bottom -= yEdgeGain;
        graphRect.top += yEdgeGain;
        regenArrayBuffer = true;
    }
}

void Graph::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftButtonDown = true;
        }
        else if (action == GLFW_RELEASE) {
            leftButtonDown = false;
        }
    }
}

void Graph::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (leftButtonDown) {
        float deltaPixel = graphRect.width / screenRect.width;

        if (xoffset != 0.0f) {
            graphRect.left -= xoffset * deltaPixel;
            graphRect.right -= xoffset * deltaPixel;
            regenArrayBuffer = true;
        }
        if (yoffset != 0.0f) {
            graphRect.bottom -= yoffset * deltaPixel;
            graphRect.top -= yoffset * deltaPixel;
            regenArrayBuffer = true;
        }
    }
}