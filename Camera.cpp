#include "Camera.h"

Camera::Camera(std::vector<GLuint> shaders, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far, bool active)
	: shaders(shaders)
	, fov(fov)
	, aspect(aspect)
	, near(near)
	, far(far)
	, eyePos(NULL)
	, forward(NULL)
	, up(NULL)
	, active(active)
{
}

Camera::~Camera()
{
}

void Camera::draw(GLuint shader, const glm::mat4& C)
{
	if (active) {
		eyePos = glm::vec3(C * glm::vec4(0, 0, 0, 1));
		forward = glm::vec3(C * glm::vec4(0, 0, -1, 0));
		up = glm::vec3(C * glm::vec4(0, 1, 0, 0));

		glm::mat4 projection = glm::perspective(fov, aspect, near, far);
		glm::mat4 view = glm::lookAt(eyePos, eyePos + forward, up);

		for (GLuint shader : shaders) {
			glUseProgram(shader);
			glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
			glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(eyePos));
			glUseProgram(0);
		}
	}
}

void Camera::update(const glm::mat4& C)
{
}