#include <graph.h>

void Graph::render() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    solidShader->use();
    
    set_solid_color(0.0f, 0.0f, 0.0f);
    glBindVertexArray(aVAO);
    glDrawArrays(GL_LINES, 0, 4);

    set_solid_color(0.0f, 0.0f, 1.0f);
    glBindVertexArray(gVAO);
    for (int i = 0; i < nameSet.size(); i++) {
        glDrawArrays(GL_LINE_STRIP, i * (numVertices / nameSet.size()), numVertices / nameSet.size());
    }
}

void Graph::set_solid_color(float r, float g, float b) {
    solidShader->use();
    solidShader->set_float(r, "r");
    solidShader->set_float(g, "g");
    solidShader->set_float(b, "b");
}
