#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Node.h"

class Camera : public Node
{
private:
	std::vector<GLuint> shaders;
public:
	Camera(std::vector<GLuint> shaders);
	~Camera();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif