#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/glad/include/glad/glad.h"

Shader::Shader(const char* filename, int type) {
    std::ifstream in(filename, std::ios_base::in);
    if (!in) {
        std::cerr << "Couldn't open file " << filename << "!" << std::endl;
    }

    std::ostringstream stream;
    stream << in.rdbuf(); // copy the contents of the file into the stream
    std::string str = stream.str();

    const char* source = str.c_str();
    // create the shader
    this->shaderId = glCreateShader(type);
    glShaderSource(this->shaderId, 1, &source, NULL);
}

bool Shader::compile() {
    int success;
    char errorDescription[256];

    glCompileShader(this->shaderId);
    glGetShaderiv(this->shaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(this->shaderId, 256, NULL, errorDescription);
        std::cerr << "Error while compiling shader: " << errorDescription << std::endl;
        return false;
    }
    return true;
}

void Shader::attach(unsigned int shaderProgram) {
    glAttachShader(shaderProgram, this->shaderId);
}

void Shader::dispose() {
    glDeleteShader(this->shaderId);
}

bool Shader::linkProgram(unsigned int shaderProgram) {
    int success;
    char errorDescription[256];

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 256, NULL, errorDescription);
        std::cerr << "Error while linking program: " << errorDescription << std::endl;
        return false;
    }
    return true;
}