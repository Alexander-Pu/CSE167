#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldPosition;
in vec2 fragTexCoordinates;
in vec3 worldNormal;

// View variables
uniform vec3 eyePos;

// Material attributes
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

// Texture
uniform sampler2D tex;

// Directional Light attributes
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight directionalLight;

// Point Light attributes
struct PointLight {
    vec3 position;
    vec3 color;
    vec3 attenuation;
};
#define NUM_POINT_LIGHTS 1
uniform PointLight pointLights[NUM_POINT_LIGHTS];

// Spot Light attributes
struct SpotLight {
    vec3 position;
    vec3 color;
    vec3 attenuation;
    vec3 direction;
    float cutoff;
    float exponent;
};
#define NUM_SPOT_LIGHTS 1
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

// Helper functions
float when_lt(float x, float y);

vec3 calc_point_light(PointLight light, vec3 worldNormal, vec3 eyeVector);
vec3 calc_spot_light(SpotLight light, vec3 worldNormal, vec3 eyeVector);

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 normalizedWorldNormal = normalize(worldNormal);
    vec3 eyeVector = normalize(eyePos - worldPosition);
    vec3 toPosEyeVector = normalize(worldPosition - eyePos);

    if (dot(eyeVector, normalizedWorldNormal) < .4) {
        fragColor = vec4(0, 0, 0, 1);
        return;
    }
    
    // ------------------------------------------------------------------------------------------------------------
    // Point light calculations
    vec3 pl_total = vec3(0, 0, 0);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        pl_total += calc_point_light(pointLights[i], normalizedWorldNormal, eyeVector);
    }

    // ------------------------------------------------------------------------------------------------------------
    // Spot light calculations
    vec3 sl_total = vec3(0, 0, 0);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        sl_total += calc_spot_light(spotLights[i], normalizedWorldNormal, toPosEyeVector);
    }

    // ------------------------------------------------------------------------------------------------------------
    vec3 totalColor = (pl_total + sl_total);

    fragColor = vec4(texture(tex, fragTexCoordinates).rgb * totalColor, 1.0f);
}


float when_lt(float x, float y) {
    return max(sign(y - x), 0);
}


vec3 calc_point_light(PointLight light, vec3 worldNormal, vec3 eyeVector) {
    vec3 pl_total = vec3(0.0, 0.0, 0.0);
    vec3 pl_vec_to_light = light.position - worldPosition;
    vec3 pl_vec_to_light_norm = normalize(pl_vec_to_light);

    float pl_dot = max(dot(pl_vec_to_light_norm, worldNormal), 0.0);
    
    vec3 pl_reflection = normalize((2 * pl_dot * worldNormal) - pl_vec_to_light_norm);
    float pl_specular = pow(max(dot(pl_reflection, eyeVector), 0.0), material.shininess); 

    float pl_distance = length(pl_vec_to_light);
    float pl_intensity = clamp(1.0 / (light.attenuation.z * pow(pl_distance, 2) + light.attenuation.y * pl_distance + light.attenuation.x), 0.0, 1.0);

    float pl_total_intensity = pl_dot * pl_specular * pl_intensity;
    
    vec3 pl_color = material.ambient;

    if (pl_total_intensity > 0.85) {
        pl_color = pl_color;
    } else if (pl_total_intensity > 0.3) {
        pl_color *= 0.95;
    } else if (pl_total_intensity > 0.00000000000001) {
        pl_color *= 0.8;
    } else {
        pl_color *= 0.7;
    }

    return light.color * pl_color;
}

vec3 calc_spot_light(SpotLight light, vec3 worldNormal, vec3 eyeVector) {
    vec3 sl_vec_to_light = light.position - worldPosition;
    vec3 sl_vec_to_light_norm = normalize(sl_vec_to_light);

    float sl_dot = max(dot(sl_vec_to_light_norm, worldNormal), 0);
    if (sl_dot <= 0) {
        return vec3(0, 0, 0);
    }
    
    // Specular color
    vec3 sl_reflection = reflect(eyeVector, worldNormal);

    float sl_specular = pow(max(dot(sl_reflection, eyeVector), 0), material.shininess); 
           
    vec3 sl_reverse_dir = -light.direction;
    float sl_dot_center = max(dot(sl_vec_to_light_norm, sl_reverse_dir), 0);
    float sl_cone_intensity = when_lt(acos(sl_dot_center), light.cutoff) * pow(sl_dot_center, light.exponent);
    float final_sl_cone_intensity = max(0, sl_cone_intensity);

    if (final_sl_cone_intensity == 0) {
        return vec3(0, 0, 0);
    }

    // Attenuation
    float sl_distance = length(sl_vec_to_light);
    float sl_intensity = clamp(1.0 / (light.attenuation.z * pow(sl_distance, 2) + light.attenuation.y * sl_distance + light.attenuation.x), 0, 1);

    float sl_total_intensity = sl_dot * sl_specular * final_sl_cone_intensity * sl_intensity;

    vec3 sl_color = material.ambient;

    if (sl_total_intensity > 0.85) {
        sl_color = sl_color;
    } else if (sl_total_intensity > 0.3) {
        sl_color *= 0.95;
    } else if (sl_total_intensity > 0.00000000000001) {
        sl_color *= 0.8;
    } else {
        sl_color *= 0.7;
    }

    return light.color * sl_color;
}