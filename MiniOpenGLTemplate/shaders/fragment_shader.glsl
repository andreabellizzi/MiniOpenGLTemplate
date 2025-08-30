#version 460 core

// Input from vertex shader
in vec3 vertexColor;

// Output color
out vec4 FragColor;

// Optional uniform for tinting
//uniform vec3 tintColor = vec3(1.0, 1.0, 1.0);

void main() {
    // Simple color output
    FragColor = vec4(vertexColor, 1.0);
}