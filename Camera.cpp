#include "Camera.h"

Camera::Camera(std::vector<GLuint> shaders, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
	: shaders(shaders)
	, fov(fov)
	, aspect(aspect)
	, near(near)
	, far(far)
{
}

Camera::~Camera()
{
}

void Camera::draw(GLuint shader, const glm::mat4& C)
{
	glm::mat4 projection = glm::perspective(fov, aspect, near, far);

	glm::vec3 eyePos = glm::vec3(C * glm::vec4(0, 0, 0, 1));
	glm::vec3 lookAt = glm::vec3(C * glm::vec4(0, 0, -1, 0));
	glm::vec3 upVector = glm::vec3(C * glm::vec4(0, 1, 0, 0));
	
	glm::mat4 view = glm::lookAt(eyePos, eyePos + lookAt, upVector);

	for (GLuint shader : shaders) {
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
		glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(eyePos));
		glUseProgram(0);
	}
}

void Camera::update()
{
}