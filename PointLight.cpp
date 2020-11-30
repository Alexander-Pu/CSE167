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

void PointLight::draw(GLuint shader, const glm::mat4& C)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shader, "pl_pos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shader, "pl_col"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shader, "pl_atten"), 1, glm::value_ptr(atten));


	glUseProgram(0);
}

void PointLight::update()
{
}