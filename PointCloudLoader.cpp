#include "PointCloudLoader.h"

PointCloudLoader::PointCloudLoader()
{
}

PointCloudLoader::~PointCloudLoader()
{
}

/*
* Reads a given file of glm::vec3 points and returns PointCloud.
* Assumes that the file is properly formatted.
*/
PointCloud* PointCloudLoader::loadPointCloud(const std::string objFileName)
{
	std::ifstream objFile(objFileName);
	std::vector<glm::vec3> points;

	GLfloat minX = 0.0;
	GLfloat maxX = 0.0;
	GLfloat minY = 0.0;
	GLfloat maxY = 0.0;
	GLfloat minZ = 0.0;
	GLfloat maxZ = 0.0;

	if (objFile.is_open())
	{
		std::string line;

		while (std::getline(objFile, line))
		{
			std::stringstream ss;
			ss << line;

			std::string label;
			ss >> label;

			if (label == "v")
			{
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Calculate min/max values
				// X
				minX = glm::min(point.x, minX);
				maxX = glm::max(point.x, maxX);
				// Y
				minY = glm::min(point.y, minY);
				maxY = glm::max(point.y, maxY);
				// Z
				minZ = glm::min(point.z, minZ);
				maxZ = glm::max(point.z, maxZ);

				points.push_back(point);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFileName << std::endl;
	}

	objFile.close();

	// Determine midpoint
	GLfloat midX = getMidpoint(minX, maxX);
	GLfloat midY = getMidpoint(minY, maxY);
	GLfloat midZ = getMidpoint(minZ, maxZ);

	glm::vec3 center = glm::vec3(midX, midY, midZ);

	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i] - center;
	}

	// Magic numbers. Declared here instead of as constants in the file due to laziness
	GLfloat defaultPointSize = 1.0;
	// These can technically be determined by using Z offset and FOV, but I don't know how to do that yet.
	GLfloat boundingDimension = 6.0;

	// Determine which axis is the bounding factor
	GLfloat widthX = glm::abs(maxX - midX);
	GLfloat widthY = glm::abs(maxY - midY);
	GLfloat widthZ = glm::abs(maxZ - midZ);

	GLfloat scalingFactorX = boundingDimension / widthX;
	GLfloat scalingFactorY = boundingDimension / widthY;
	GLfloat scalingFactorZ = boundingDimension / widthZ;

	GLfloat minScalingFactor = glm::min(glm::min(scalingFactorX, scalingFactorY), scalingFactorZ);

	// Scale all values
	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i] * minScalingFactor;
	}

	return new PointCloud(points, defaultPointSize);
}

GLfloat PointCloudLoader::getMidpoint(const GLfloat min, const GLfloat max) {
	return (min + max) / 2.0f;
}
