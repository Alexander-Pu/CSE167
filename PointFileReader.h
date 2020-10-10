#ifndef _POINT_FILE_READER_H_
#define _POINT_FILE_READER_H_

#include "Object.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class PointFileReader
{

public:
	PointFileReader();
	~PointFileReader();

	std::vector<glm::vec3> readFile(const std::string objFileName);
};

#endif