#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Node.h"

class Camera : public Node
{
public:
	Camera();
	~Camera();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif