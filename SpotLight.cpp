#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten, glm::vec3 direction,float cutoff, float exponent)
	: pos(pos)
	, color(color)
	, atten(atten)
	, direction(direction)
	, cutoff(cutoff)
	, exponent(exponent)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::draw(GLuint shader, const glm::mat4& C)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shader, "sl_pos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shader, "sl_col"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shader, "sl_atten"), 1, glm::value_ptr(atten));
	glUniform3fv(glGetUniformLocation(shader, "sl_dir"), 1, glm::value_ptr(direction));
	glUniform1f(glGetUniformLocation(shader, "sl_cutoff"), cutoff);
	glUniform1f(glGetUniformLocation(shader, "sl_exp"), exponent);

	glUseProgram(0);
}

void SpotLight::update()
{
}
