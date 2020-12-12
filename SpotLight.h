#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "Node.h"

class SpotLight : public Node
{
private:
	glm::vec3 color, atten;
	float cutoff, exponent;
	std::vector<GLuint> shaders;

public:
	SpotLight(std::vector<GLuint> shaders, glm::vec3 color, glm::vec3 atten, float cutoff, float exponent);
	~SpotLight();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
};

#endif
