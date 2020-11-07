#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is the vertex normal vertex shader.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 color;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPosition;

    // Set color based on vertex normal
    // Matrices are reflected across diagonal due to column-major orientation.
    mat4 add2Matrix = mat4(1.0, 0.0, 0.0, 0.0, 
					       0.0, 1.0, 0.0, 0.0, 
					       0.0, 0.0, 1.0, 0.0,  
					       1.0, 1.0, 1.0, 1.0);
	mat4 scaleHalfMatrix = mat4(0.5, 0.0, 0.0, 0.0, 
				                0.0, 0.5, 0.0, 0.0, 
				                0.0, 0.0, 0.5, 0.0, 
				                0.0, 0.0, 0.0, 1.0);
    color = vec3(scaleHalfMatrix * add2Matrix * vec4(normal, 1.0));
}