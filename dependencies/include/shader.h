#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>

class Shader {
public:
    Shader(const char* vertexFP, const char* fragmentFP);

    void use();

    void set_float(float f, const char* name);
    void set_mat4(glm::mat4* m, const char* name);

private:
    unsigned int ID;
};