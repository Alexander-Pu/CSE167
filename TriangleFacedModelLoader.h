#ifndef _TRIANGLE_FACED_MODEL_LOADER_H_
#define _TRIANGLE_FACED_MODEL_LOADER_H_

#include "TriangleFacedModel.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class TriangleFacedModelLoader
{
private:
	GLfloat getMidpoint(const GLfloat min, const GLfloat max);
	const std::string PAIR_DELIMITER = "//";

public:
	TriangleFacedModelLoader();
	~TriangleFacedModelLoader();

	TriangleFacedModel* loadTriangleFacedModel(const std::string objFileName);
};

#endif