#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalModel;

out vec3 worldPosition;
out vec2 fragTexCoordinates;
out vec3 worldNormal;

void main()
{
    worldPosition = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * vec4(worldPosition, 1.0);

    // Pass through texture coordinates
    fragTexCoordinates = textureCoordinates;

    worldNormal = normalModel * normal;
}