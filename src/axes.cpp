#include "axes.h"

Axes::Axes() {
    for (int i = 0; i < 8; i++) {
        coordsArray[i] = 0.0f;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), coordsArray, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Axes::render(Shader* solidShader) {
    solidShader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);
}

void Axes::calculate_axes(Rectf graphRect) {
    //Top Y
    coordsArray[1] = graphRect.top;
    //Bottom Y
    coordsArray[3] = graphRect.bottom;
    //Left X
    coordsArray[4] = graphRect.left;
    //Right X
    coordsArray[6] = graphRect.right;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), coordsArray);
}