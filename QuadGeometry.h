#ifndef _QUAD_GEOMETRY_H_
#define _QUAD_GEOMETRY_H_

#include "Geometry.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <sstream>

class QuadGeometry : public Geometry
{
private:
	void pushFaceIndices(std::string faceVertex, std::vector<int>& vertexIndices, std::vector<int>& textureCoordinateIndices, std::vector<int>& vertexNormalIndices);

public:
	QuadGeometry(const std::string objFileName, Texture* texture);
	~QuadGeometry();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif