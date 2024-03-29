#version 330 core
layout (location = 0) in vec3 position;

out vec3 texture_coords;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 eyePos;
uniform mat4 model;

void main()
{
    texture_coords = position;
    gl_Position = projection * view * model * vec4(position, 1.0);
}  