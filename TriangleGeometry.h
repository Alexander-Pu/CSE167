#ifndef _TRIANGLE_GEOMETRY_H_
#define _TRIANGLE_GEOMETRY_H_

#include "Geometry.h"

#include <iostream>
#include <fstream>
#include <sstream>

class TriangleGeometry : public Geometry
{
private:
	void pushFaceIndices(std::string faceVertex, std::vector<int>& vertexIndices, std::vector<int>& textureCoordinateIndices, std::vector<int>& vertexNormalIndices);

public:
	TriangleGeometry(const std::string objFileName, Texture* newTexture);
	~TriangleGeometry();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif