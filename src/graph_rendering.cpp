#include <graph.h>

void Graph::render() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    solidShader->use();
    
    set_solid_color(glm::vec3(0.0f, 0.0f, 0.0f));
    glBindVertexArray(aVAO);
    glDrawArrays(GL_LINES, 0, 4);

    glBindVertexArray(gVAO);
    for (int i = 0; i < renderOrder.size(); i++) {
        std::string name = renderOrder.at(i);
        set_solid_color(nameToColor[name]);
        glDrawArrays(GL_LINE_STRIP, i * (numVertices / nameSet.size()), numVertices / nameSet.size());
    }
}

void Graph::set_solid_color(glm::vec3 rgb) {
    solidShader->use();
    solidShader->set_float(rgb.x, "r");
    solidShader->set_float(rgb.y, "g");
    solidShader->set_float(rgb.z, "b");
}
