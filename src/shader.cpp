#include <shader.h>

Shader::Shader(const char* vertexFP, const char* fragmentFP) {
    unsigned int vertexShader, fragmentShader;
    std::ifstream inVertexFile(vertexFP), inFragmentFile(fragmentFP);
    std::string file = "", line = "";

    if (inVertexFile.is_open()) {
        while (std::getline(inVertexFile, line)) {
            file.append(line.append("\n"));
        }
        inVertexFile.close();
    }
    else {
        std::cout << "Failed to open " << vertexFP << std::endl;
        return;
    }
    const char* vcstr = file.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vcstr, 0);
    glCompileShader(vertexShader);

    file = "";
    if (inFragmentFile.is_open()) {
        while (std::getline(inFragmentFile, line)) {
            file.append(line.append("\n"));
        }
    }
    else {
        std::cout << "Failed to open " << fragmentFP << std::endl;
        return;
    }
    const char* fcstr = file.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fcstr, 0);
    glCompileShader(fragmentShader);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    ID = shaderProgram;
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::set_float(float f, const char* name) {
    int location = glGetUniformLocation(ID, name);
    glUniform1f(location, f);
}

void Shader::set_mat4(glm::mat4* m, const char* name) {
    int location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*m));
}
