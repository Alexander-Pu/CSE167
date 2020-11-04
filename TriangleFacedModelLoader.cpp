#include "TriangleFacedModelLoader.h"

TriangleFacedModelLoader::TriangleFacedModelLoader()
{
}

TriangleFacedModelLoader::~TriangleFacedModelLoader()
{
}

/*
* Reads a given file of vertices, vertex normals, and face definitions, and returns TriangleFacedModel.
* Assumes that the file is properly formatted.
*/
TriangleFacedModel* TriangleFacedModelLoader::loadTriangleFacedModel(const std::string objFileName)
{
	std::ifstream objFile(objFileName);
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::uvec3> vertexIndexes;
	std::vector<glm::uvec3> vertexNormalIndexes;

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

				vertexes.push_back(point);
			}
			else if (label == "vn") {
				glm::vec3 vertexNormal;
				ss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;

				// Normalize all the normals
				vertexNormals.push_back(glm::normalize(vertexNormal));
			}
			else if (label == "f") {
				std::string pair;

				glm::vec3 vertexIndexTriple;
				glm::vec3 vertexNormalIndexTriple;

				// First index pair
				ss >> pair;
				int delimiterIdx = pair.find(PAIR_DELIMITER);
				vertexIndexTriple.x = std::stoi(pair.substr(0, delimiterIdx)) - 1;
				vertexNormalIndexTriple.x = std::stoi(pair.substr(delimiterIdx + PAIR_DELIMITER.length())) - 1;
				
				// Second index pair
				ss >> pair;
				delimiterIdx = pair.find(PAIR_DELIMITER);
				vertexIndexTriple.y = std::stoi(pair.substr(0, delimiterIdx)) - 1;
				vertexNormalIndexTriple.y = std::stoi(pair.substr(delimiterIdx + PAIR_DELIMITER.length())) - 1;

				// Third index pair
				ss >> pair;
				delimiterIdx = pair.find(PAIR_DELIMITER);
				vertexIndexTriple.z = std::stoi(pair.substr(0, delimiterIdx)) - 1;
				vertexNormalIndexTriple.z = std::stoi(pair.substr(delimiterIdx + PAIR_DELIMITER.length())) - 1;
				
				vertexIndexes.push_back(vertexIndexTriple);
				vertexNormalIndexes.push_back(vertexNormalIndexTriple);
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

	for (int i = 0; i < vertexes.size(); i++) {
		vertexes[i] = vertexes[i] - center;
	}

	// Magic numbers. Declared here instead of as constants in the file due to laziness
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
	for (int i = 0; i < vertexes.size(); i++) {
		vertexes[i] = vertexes[i] * minScalingFactor;
	}

	return new TriangleFacedModel(vertexes, vertexNormals, vertexIndexes, vertexNormalIndexes);
}

GLfloat TriangleFacedModelLoader::getMidpoint(const GLfloat min, const GLfloat max) {
	return (min + max) / 2.0f;
}
