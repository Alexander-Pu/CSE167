#version 330 core
out vec4 FragColor;

in vec3 worldPosition;
in vec3 worldNormal;

uniform samplerCube skybox;
uniform vec3 eyePos;

void main()
{
    vec3 eyeVec = normalize(worldPosition - eyePos);
    vec3 reflectVec = reflect(eyeVec, normalize(worldNormal));

    FragColor = vec4(texture(skybox, reflectVec).rgb, 1.0);
}