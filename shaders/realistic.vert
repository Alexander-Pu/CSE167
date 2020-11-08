#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is the vertex normal vertex shader.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalModel;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 worldPosition;
out vec3 worldNormal;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    worldPosition = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * vec4(worldPosition, 1.0);

    worldNormal = normalModel * normal;
}