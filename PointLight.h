#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Node.h"

class PointLight : public Node
{
private:
	glm::vec3 pos, color, atten;

public:
	PointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten);
	~PointLight();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif
