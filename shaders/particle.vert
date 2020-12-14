#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 particleColor;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform vec3 eyePos;

const float minPoint = 2.0;
const float maxPoint = 10.0;
const float factor = 100.0;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 color;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * vec4(position, 1.0);

    float cameraDist = distance(position.xyz, eyePos);
    float pointScale = factor / cameraDist;
    pointScale = max(pointScale, minPoint);
    pointScale = min(pointScale, maxPoint);

    gl_PointSize = pointScale;

    color = particleColor;
}