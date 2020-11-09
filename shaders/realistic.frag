#version 330 core
// This is the vertex normal fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldPosition;
in vec3 worldNormal;

// View variables
uniform mat4 view;

// Material attributes
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

// Light attributes
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 lightAtten;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 lightVectorNonNormalized = lightPos - worldPosition;
    vec3 lightVector = normalize(lightVectorNonNormalized);

    vec3 normalizedWorldNormal = normalize(worldNormal);
    float lightDotNormal = dot(lightVector, normalizedWorldNormal);

    // Diffuse color
    vec3 diffuseColor = diffuse * max(lightDotNormal, 0.0);
    
    // Specular color
    vec3 reflectionVector = (2 * lightDotNormal * normalizedWorldNormal) - lightVector;
    vec3 eyePos = vec3(-view[3][0], -view[3][1], -view[3][2]);
    vec3 eyeVector = normalize(eyePos - worldPosition);

    vec3 specularColor = specular * max(pow(dot(reflectionVector, eyeVector), shininess), 0.0); 

    // Attenuation
    float lightDistance = length(lightVectorNonNormalized);
    vec3 attentuatedColor = lightCol / (lightAtten.z * pow(lightDistance, 2) + lightAtten.y * lightDistance + lightAtten.x);

    // Summation of colors
    vec3 totalColor = attentuatedColor * (diffuseColor + specularColor) + ambient;

    fragColor = vec4(totalColor, 1.0f);
}