#ifndef _DISCO_BALL_H_
#define _DISCO_BALL_H_

#include "Geometry.h"

#include <vector>
#include <iostream>

using namespace std;

class DiscoBall : public Geometry
{
public:
	DiscoBall(int numFacesVertical, int numFacesHorizontal);
	~DiscoBall();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
	void setTexture(CubeMapTexture* newTexture);
};

#endif