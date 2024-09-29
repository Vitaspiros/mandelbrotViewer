#include "../include/glad/include/glad/glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 900
#define TITLE "Mandelbrot Viewer"

// DATA
// vertices for rectangle
float vertices[] = {
    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f
};

// indices for rectangle
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

double scalar = 1.0f;
int sign = 1;
bool isZooming = false;
double xOffset = 0.0f;
double yOffset = 0.0f;

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        isZooming = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT) sign = 1;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) sign = -1;
    } else if (action == GLFW_RELEASE) {
        isZooming = false;
    }
}

void processMouseClick(GLFWwindow* window) {
    if (isZooming) {
        scalar += 0.01 * scalar * sign;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xOffset += ((xpos-WIDTH/2)/WIDTH)/(4*scalar);
        yOffset += ((ypos-HEIGHT/2)/HEIGHT)/(4*scalar);
    }
}

int main() {
    if (!glfwInit()) {
        const char* description = new char[256];
        glfwGetError(&description);
        std::cerr << "Couldn't initialize GLFW: " << description << std::endl;
        return -1;
    }

    // set OpenGL profile to core and version to 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Couldn't create window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Couldn't initialize GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetMouseButtonCallback(window, mouseClickCallback);

    // VERTEX ARRAY OBJECT
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // BUFFERS
    unsigned int VBO; // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy the vertices to the buffer

    unsigned int EBO; // Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // copy the indices to the buffer
    
    // SHADERS
    unsigned int shaderProgram = glCreateProgram();
    // Vertex Shader
    Shader vertexShader("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
    vertexShader.compile();
    vertexShader.attach(shaderProgram);

    // Fragment Shader
    Shader fragmentShader("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    fragmentShader.compile();
    fragmentShader.attach(shaderProgram);

    // Link the program
    Shader::linkProgram(shaderProgram);
    vertexShader.dispose();   // delete shader
    fragmentShader.dispose(); // delete shader
    glUseProgram(shaderProgram);

    // VERTEX ARRAY LINKING
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    while (!glfwWindowShouldClose(window)) {
        processMouseClick(window);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        // set the uniform values
        glUniform1d(glGetUniformLocation(shaderProgram, "fractalScalar"), 1/scalar);
        glUniform1d(glGetUniformLocation(shaderProgram, "zoomingOffsetX"), xOffset);
        glUniform1d(glGetUniformLocation(shaderProgram, "zoomingOffsetY"), yOffset);
        // draw using the EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // free memory and terminate
    glBindVertexArray(0); // unbind the VAO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}