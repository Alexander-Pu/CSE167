#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 eyePos;
uniform float focalLength;
uniform float origPointSize;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPosition;
    sampleExtraOutput = 1.0f;

    // Calculate distance, with some min value to protect from division by 0
    float distanceFromEye = max(distance(worldPosition, eyePos), 0.1f);

    // Scale size based on focal length and distance from eye
    // based on thin lens equation
    float newPointSize = origPointSize * (focalLength / (distanceFromEye - focalLength));

    // Set point size with a minimum of 1.0f since we don't want points to disappear
    gl_PointSize = max(newPointSize, 1.0f);
}