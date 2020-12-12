#include "SpotLight.h"

SpotLight::SpotLight(std::vector<GLuint> shaders, glm::vec3 color, glm::vec3 atten,float cutoff, float exponent)
	: shaders(shaders)
	, color(color)
	, atten(atten)
	, cutoff(cutoff)
	, exponent(exponent)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::draw(GLuint shader, const glm::mat4& C)
{
	for (GLuint shader : shaders) {
		// Activate the shader program 
		glUseProgram(shader);

		glm::vec3 position = glm::vec3(C * glm::vec4(0, 0, 0, 1));
		glm::vec3 lookAt = glm::normalize(glm::vec3(C * glm::vec4(0, -1, 0, 0)));

		// Get the shader variable locations and send the uniform data to the shader 
		glUniform3fv(glGetUniformLocation(shader, "spotLights[0].position"), 1, glm::value_ptr(position));
		glUniform3fv(glGetUniformLocation(shader, "spotLights[0].color"), 1, glm::value_ptr(color));
		glUniform3fv(glGetUniformLocation(shader, "spotLights[0].atten"), 1, glm::value_ptr(atten));
		glUniform3fv(glGetUniformLocation(shader, "spotLights[0].direction"), 1, glm::value_ptr(lookAt));
		glUniform1f(glGetUniformLocation(shader, "spotLights[0].cutoff"), cutoff);
		glUniform1f(glGetUniformLocation(shader, "spotLights[0].exponent"), exponent);

		glUseProgram(0);
	}
}

void SpotLight::update(const glm::mat4& C)
{
}
