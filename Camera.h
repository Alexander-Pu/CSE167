#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Node.h"

class Camera : public Node
{
private:
	std::vector<GLuint> shaders;
	GLfloat fov;
	GLfloat aspect;
	GLfloat near;
	GLfloat far;
public:
	Camera(std::vector<GLuint> shaders, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);
	~Camera();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
	GLfloat getFov() { return fov; }
	void setFov(GLfloat newFov) { fov = newFov; }
	void setAspect(GLfloat newAspect) { aspect = newAspect; }
};

#endif