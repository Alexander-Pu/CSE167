#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "CubeMapTexture.h"

class Geometry : public Node
{
protected:
	int numIndices;
	int numVertices;

	GLuint VAO;
	GLuint VBO[3], EBO[2];

public:
	virtual void draw(GLuint shader, const glm::mat4& C) = 0;
	virtual void update() = 0;
};

#endif