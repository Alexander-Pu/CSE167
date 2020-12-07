#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "Materials.h"
#include "Texture.h"

class Geometry : public Node
{
protected:
	int numIndices;
	int numVertices;
	Materials* materials = NULL;
	Texture* texture = NULL;

	GLuint VAO;
	GLuint VBO[3], EBO;

public:
	virtual void draw(GLuint shader, const glm::mat4& C) = 0;
	virtual void update() = 0;
	void setMaterials(Materials* newMaterials) { materials = newMaterials; }
	void setTexture(Texture* newTexture) { texture = newTexture; }
};

#endif