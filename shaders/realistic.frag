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

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 normalizedWorldNormal = normalize(worldNormal);
    vec3 eyePos = vec3(-view[3][0], -view[3][1], -view[3][2]);
    vec3 eyeVector = normalize(eyePos - worldPosition);
    
    // ------------------------------------------------------------------------------------------------------------
    // Point light calculations
    vec3 pl_vec_to_light = pl_pos - worldPosition;
    vec3 pl_vec_to_light_norm = normalize(pl_vec_to_light);

    float pl_dot = dot(pl_vec_to_light_norm, normalizedWorldNormal);

    // Diffuse color
    vec3 pl_diffuse = diffuse * max(pl_dot, 0.0);
    
    // Specular color
    vec3 pl_reflection = (2 * pl_dot * normalizedWorldNormal) - pl_vec_to_light_norm;
    vec3 pl_specular = specular * max(pow(dot(pl_reflection, eyeVector), shininess), 0.0); 

    // Attenuation
    float pl_distance = length(pl_vec_to_light);
    vec3 pl_faded = pl_col  / (pl_atten.z * pow(pl_distance, 2) + pl_atten.y * pl_distance + pl_atten.x);
    vec3 pl_total = pl_faded * (pl_diffuse + pl_specular);

    // ------------------------------------------------------------------------------------------------------------
    // Spot light calculations
    vec3 sl_vec_to_light = sl_pos - worldPosition;
    vec3 sl_vec_to_light_norm = normalize(sl_vec_to_light);

    float sl_dot = dot(sl_vec_to_light_norm, normalizedWorldNormal);

    // Diffuse color
    vec3 sl_diffuse = diffuse * max(sl_dot, 0.0);
    
    // Specular color
    vec3 sl_reflection = (2 * sl_dot * normalizedWorldNormal) - sl_vec_to_light_norm;
    vec3 sl_specular = specular * max(pow(dot(sl_reflection, eyeVector), shininess), 0.0); 
           
    float sl_intensity = 0.0;
    vec3 sl_reverse_dir = -sl_dir;
    float sl_dot_center = dot(sl_vec_to_light_norm, sl_reverse_dir);
    // Within cone
    if (acos(sl_dot_center) <= sl_cutoff) {
        sl_intensity = pow(sl_dot_center, sl_exp);
    }
    vec3 sl_cone_col = sl_intensity * sl_col;

    // Attenuation
    float sl_distance = length(sl_vec_to_light);
    vec3 sl_faded = sl_cone_col  / (sl_atten.z * pow(sl_distance, 2) + sl_atten.y * sl_distance + sl_atten.x);
    vec3 sl_total = sl_faded * (sl_diffuse + sl_specular);

    // ------------------------------------------------------------------------------------------------------------
    vec3 totalColor = (pl_total + sl_total + ambient);

    fragColor = vec4(totalColor, 1.0f);
}