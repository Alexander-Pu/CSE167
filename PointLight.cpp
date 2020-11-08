#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten)
	: pos(pos)
	, color(color)
	, atten(atten)
{
}

PointLight::~PointLight()
{
}

// Assumes this is being called in a method that is NOT already calling glUseProgram()
void PointLight::sendLightToShader(const int shaderID) {
	// Activate the shader program 
	glUseProgram(shaderID);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shaderID, "lightPos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shaderID, "lightCol"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "lightAtten"), 1, glm::value_ptr(atten));

	glUseProgram(0);
}
