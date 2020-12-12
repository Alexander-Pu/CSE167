#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Node.h"

class PointLight : public Node
{
private:
	std::vector<GLuint> shaders;
	glm::vec3 pos, color, atten;

public:
	PointLight(std::vector<GLuint> shaders, glm::vec3 pos, glm::vec3 color, glm::vec3 atten);
	~PointLight();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
};

#endif
