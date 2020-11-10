#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 direction, glm::vec3 atten, float cutoff, float exponent)
	: pos(pos)
	, color(color)
	, direction(direction)
	, atten(atten)
	, cutoff(cutoff)
	, exponent(exponent)
{
}

SpotLight::~SpotLight()
{
}

// Assumes this is being called in a method that is NOT already calling glUseProgram()
void SpotLight::sendLightToShader(const int shaderID) {
	// Activate the shader program 
	glUseProgram(shaderID);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shaderID, "sl_pos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shaderID, "sl_col"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "sl_dir"), 1, glm::value_ptr(direction));
	glUniform3fv(glGetUniformLocation(shaderID, "sl_atten"), 1, glm::value_ptr(atten));
	glUniform1f(glGetUniformLocation(shaderID, "sl_cutoff"), cutoff);
	glUniform1f(glGetUniformLocation(shaderID, "sl_exp"), exponent);

	glUseProgram(0);
}

void SpotLight::setPos(glm::vec3 newPos) {
	pos = newPos;
}

void SpotLight::setDirection(glm::vec3 newDirection) {
	direction = newDirection;
}

void SpotLight::clear(const int shaderID) {
	// Activate the shader program 
	glUseProgram(shaderID);

	// Set the color to 0.
	glUniform3fv(glGetUniformLocation(shaderID, "sl_col"), 1, glm::value_ptr(glm::vec3(0.0)));

	glUseProgram(0);
}
