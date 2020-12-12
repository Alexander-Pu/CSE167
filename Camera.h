#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Node.h"

class Camera : public Node
{
private:
	std::vector<GLuint> shaders;
	GLfloat fov, aspect, near, far;
	glm::vec3 eyePos, forward, up;
	bool active;
public:
	Camera(std::vector<GLuint> shaders, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far, bool active);
	~Camera();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
	GLfloat getFov() { return fov; }
	glm::vec3 getForward() { return forward; }
	void setFov(GLfloat newFov) { fov = newFov; }
	void setAspect(GLfloat newAspect) { aspect = newAspect; }
	void setActive(bool isActive) { active = isActive; }
};

#endif