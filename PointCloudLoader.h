#ifndef _POINT_FILE_READER_H_
#define _POINT_FILE_READER_H_

#include "PointCloud.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class PointCloudLoader
{

public:
	PointCloudLoader();
	~PointCloudLoader();

	PointCloud* loadPointCloud(const std::string objFileName);
};

#endif