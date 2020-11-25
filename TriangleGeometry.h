#ifndef _TRIANGLE_GEOMETRY_H_
#define _TRIANGLE_GEOMETRY_H_

#include "Geometry.h"

#include <iostream>
#include <fstream>
#include <sstream>

class TriangleGeometry : public Geometry
{
private:
	const std::string PAIR_DELIMITER = "//";

public:
	TriangleGeometry(const std::string objFileName);
	~TriangleGeometry();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif