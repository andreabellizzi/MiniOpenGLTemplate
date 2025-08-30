#version 460 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;    // Vertex position
layout (location = 1) in vec3 aColor;  // Vertex color

// Output to fragment shader
out vec3 vertexColor;

// Uniforms (set from C++ code)
uniform mat4 projection;  // Camera projection matrix
uniform mat4 view;        // Camera view matrix  
uniform mat4 model;       // Object transform matrix

void main() {
    // Transform vertex position to screen space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass color to fragment shader
    vertexColor = aColor;
}