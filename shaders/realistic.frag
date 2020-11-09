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

// Point light attributes
uniform vec3 pointLightPos;
uniform vec3 pointLightCol;
uniform vec3 pointLightAtten;

// Spot light attributes
uniform vec3 spotLightPos;
uniform vec3 spotLightCol;
uniform vec3 spotLightDirection;
uniform vec3 spotLightAtten;
uniform float spotLightCutoff;
uniform float spotLightExponent;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 normalizedWorldNormal = normalize(worldNormal);

    // Point light calculations
    vec3 lightVectorNonNormalized = pointLightPos - worldPosition;
    vec3 lightVector = normalize(lightVectorNonNormalized);

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
    vec3 attentuatedColor = pointLightCol / (pointLightAtten.z * pow(lightDistance, 2) + pointLightAtten.y * lightDistance + pointLightAtten.x);

    // Summation of point light colors
    vec3 pointLightTotalColor = attentuatedColor * (diffuseColor + specularColor) + ambient;

    // ------------------------------------------------------------------------------------------------------------
    // Spot light calculations
    vec3 spotLightVectorNonNormalized = spotLightPos - worldPosition;
    vec3 spotLightVector = normalize(spotLightVectorNonNormalized);

    vec3 spotLightDirectionOpposite = normalize(-spotLightDirection);

    float spotLightIntensity = 0.0;
    // Within cone
    if (dot(spotLightVector, spotLightDirectionOpposite) > spotLightCutoff) {
        spotLightIntensity = max(dot(normalizedWorldNormal, spotLightVector), 0.0);

        if (spotLightIntensity > 0.0) {
            
        }
    }

    fragColor = vec4(pointLightTotalColor, 1.0f);
}