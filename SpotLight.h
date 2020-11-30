#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "Node.h"

class SpotLight : public Node
{
private:
	glm::vec3 pos, color, direction, atten;
	float cutoff, exponent;

public:
	SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten, glm::vec3 direction, float cutoff, float exponent);
	~SpotLight();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif
