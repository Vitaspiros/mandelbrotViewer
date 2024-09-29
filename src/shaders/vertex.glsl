#version 440 core
layout (location = 0) in vec3 inPos;

out vec3 colorVertex;

void main() {
    gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);
    colorVertex = inPos;
}