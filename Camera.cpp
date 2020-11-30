#include "Camera.h"

Camera::Camera(std::vector<GLuint> shaders)
	:shaders(shaders)
{
}

Camera::~Camera()
{
}

void Camera::draw(GLuint shader, const glm::mat4& C)
{
	glm::vec3 eyePos = glm::vec3(C * glm::vec4(0, 0, 0, 1));
	glm::vec3 lookAt = glm::vec3(C * glm::vec4(0, 0, -1, 0));
	glm::vec3 upVector = glm::vec3(C * glm::vec4(0, 1, 0, 0));
	
	glm::mat4 view = glm::lookAt(eyePos, eyePos + lookAt, upVector);

	for (GLuint shader : shaders) {
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
		glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(eyePos));
		glUseProgram(0);
	}
}

void Camera::update()
{
}