#include "PointLight.h"

PointLight::PointLight(std::vector<GLuint> shaders, glm::vec3 pos, glm::vec3 color, glm::vec3 atten)
	: shaders(shaders)
	, pos(pos)
	, color(color)
	, atten(atten)
{
}

PointLight::~PointLight()
{
}

void PointLight::draw(GLuint shader, const glm::mat4& C)
{
	for (GLuint shader : shaders) {
		// Activate the shader program 
		glUseProgram(shader);

		glm::vec3 position = glm::vec3(C * glm::vec4(pos, 1));

		// Get the shader variable locations and send the uniform data to the shader 
		glUniform3fv(glGetUniformLocation(shader, "pointLights[0].position"), 1, glm::value_ptr(position));
		glUniform3fv(glGetUniformLocation(shader, "pointLights[0].color"), 1, glm::value_ptr(color));
		glUniform3fv(glGetUniformLocation(shader, "pointLights[0].attenuation"), 1, glm::value_ptr(atten));

		glUseProgram(0);
	}
}

void PointLight::update(const glm::mat4& C)
{
}