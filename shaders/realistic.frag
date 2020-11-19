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
uniform vec3 pl_pos;
uniform vec3 pl_col;
uniform vec3 pl_atten;

// Spot light attributes
uniform vec3 sl_pos;
uniform vec3 sl_col;
uniform vec3 sl_dir;
uniform vec3 sl_atten;
uniform float sl_cutoff;
uniform float sl_exp;

// Helper functions
float when_lt(float x, float y);
float when_false(float bool_val);

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 normalizedWorldNormal = normalize(worldNormal);
    vec3 eyePos = vec3(-view[3][0], -view[3][1], -view[3][2]);
    vec3 eyeVector = normalize(eyePos - worldPosition);
    
    // ------------------------------------------------------------------------------------------------------------
    // Point light calculations
    vec3 pl_total = vec3(0.0, 0.0, 0.0);
    vec3 pl_vec_to_light = pl_pos - worldPosition;
    vec3 pl_vec_to_light_norm = normalize(pl_vec_to_light);

    float pl_dot = max(dot(pl_vec_to_light_norm, normalizedWorldNormal), 0.0);
    if (pl_dot > 0.0) {
        // Diffuse color
        vec3 pl_diffuse = diffuse * pl_dot;
    
        // Specular color
        vec3 pl_reflection = normalize((2 * pl_dot * normalizedWorldNormal) - pl_vec_to_light_norm);
        vec3 pl_specular = specular * pow(max(dot(pl_reflection, eyeVector), 0.0), shininess); 

        // Attenuation
        float pl_distance = length(pl_vec_to_light);
        float pl_intensity = clamp(1.0 / (pl_atten.z * pow(pl_distance, 2) + pl_atten.y * pl_distance + pl_atten.x), 0.0, 1.0);
        pl_total = pl_col * pl_intensity * (pl_diffuse + pl_specular);
    }

    // ------------------------------------------------------------------------------------------------------------
    // Spot light calculations
    vec3 sl_total = vec3(0, 0, 0);
    vec3 sl_vec_to_light = sl_pos - worldPosition;
    vec3 sl_vec_to_light_norm = normalize(sl_vec_to_light);

    float sl_dot = max(dot(sl_vec_to_light_norm, normalizedWorldNormal), 0);
    if (sl_dot > 0) {
        // Diffuse color
        vec3 sl_diffuse = diffuse * sl_dot;
    
        // Specular color
        vec3 sl_reflection = (2 * sl_dot * normalizedWorldNormal) - sl_vec_to_light_norm;
        vec3 sl_specular = specular * pow(max(dot(sl_reflection, eyeVector), 0), shininess); 
           
        float placeholder_boolean = 0.0;
        vec3 sl_reverse_dir = -sl_dir;
        float sl_dot_center = max(dot(sl_vec_to_light_norm, sl_reverse_dir), 0);
        // Within cone
        // float sl_cone_intensity = 0.0;
        // if (acos(sl_dot_center) <= sl_cutoff) {
        //     sl_cone_intensity = pow(sl_dot_center, sl_exp);
        // }
        float sl_cone_intensity = when_lt(acos(sl_dot_center), sl_cutoff) * pow(sl_dot_center, sl_exp);
        float final_sl_cone_intensity = max(when_false(placeholder_boolean), sl_cone_intensity);

        // Attenuation
        float sl_distance = length(sl_vec_to_light);
        float sl_intensity = clamp(1.0 / (sl_atten.z * pow(sl_distance, 2) + sl_atten.y * sl_distance + sl_atten.x), 0, 1);
        sl_total = sl_col * sl_intensity * final_sl_cone_intensity * (sl_diffuse + sl_specular);
    }

    // ------------------------------------------------------------------------------------------------------------
    vec3 totalColor = (pl_total + sl_total + ambient);

    fragColor = vec4(totalColor, 1.0f);
}

float when_lt(float x, float y) {
    return max(sign(y - x), 0.0);
}


float when_false(float bool_val) {
    return 1.0 - bool_val;
}