#include "PointFileReader.h"

PointFileReader::PointFileReader()
{
}

PointFileReader::~PointFileReader()
{
}

/*
* Reads a given file of glm::vec3 points and returns it as a vector.
* Assumes that the file is properly formatted.
*/
std::vector<glm::vec3> PointFileReader::readFile(const std::string objFileName)
{
	std::ifstream objFile(objFileName);
	std::vector<glm::vec3> points;

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

				//std::cout << "point: " << point.x << " " << point.y << " " << point.z << "\n";
				points.push_back(point);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFileName << std::endl;
	}

	objFile.close();

	return points;
}
