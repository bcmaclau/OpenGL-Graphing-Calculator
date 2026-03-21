#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "rectf.h"

#include <iostream>

class Axes {
public:
    Axes();
    void render(Shader* solidShader);
    void calculate_axes(Rectf graphRect);

private:
    float coordsArray[8];

    unsigned int VAO, VBO;
};
