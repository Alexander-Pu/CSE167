#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "Materials.h"
#include "Texture.h"

class Geometry : public Node
{
protected:
	float minX, maxX, minY, maxY, minZ, maxZ = 0.0;
	int numVertices;
	Materials* materials = NULL;
	Texture* texture = NULL;

	GLuint VAO;
	GLuint VBO[3], EBO;

public:
	void setMaterials(Materials* newMaterials) { materials = newMaterials; }
	void setTexture(Texture* newTexture) { texture = newTexture; }
	float getMinX() { return minX; }
	float getMaxX() { return maxX; }
	float getMinY() { return minY; }
	float getMaxY() { return maxY; }
	float getMinZ() { return minZ; }
	float getMaxZ() { return maxZ; }
};

#endif