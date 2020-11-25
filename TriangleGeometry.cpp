#include "TriangleGeometry.h"

TriangleGeometry::TriangleGeometry(const std::string objFileName)
{
	// ------------------------- LOAD GEOMETRY FROM FILE -------------------------
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
	GLfloat midX = (minX + maxX) / 2;
	GLfloat midY = (minY + maxY) / 2;
	GLfloat midZ = (minZ + maxZ) / 2;

	glm::vec3 center = glm::vec3(midX, midY, midZ);

	for (int i = 0; i < vertexes.size(); i++) {
		vertexes[i] = vertexes[i] - center;
	}

	// Bind the model to a 1x1x1 box
	GLfloat boundingDimension = 1.0;

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

	numIndices = 3 * vertexIndexes.size();

	// ------------------------- FINISHED LOADING -------------------------


	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* vertexes.size(), vertexes.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* vertexNormals.size(), vertexNormals.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass the vertex normal data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(2, &EBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)* vertexIndexes.size(), vertexIndexes.data(), GL_STATIC_DRAW);

	// Generate EBO, bind the EBO to the bound VAO, and send the vertex normal index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)* vertexNormalIndexes.size(), vertexNormalIndexes.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TriangleGeometry::~TriangleGeometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(2, &EBO[0]);
	glDeleteVertexArrays(1, &VAO);
}

void TriangleGeometry::draw(GLuint shader, const glm::mat4& C)
{
	glUseProgram(shader);

	glm::highp_mat3 normalMatrix = glm::highp_mat3(glm::transpose(glm::inverse(C)));

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));
	glUniformMatrix3fv(glGetUniformLocation(shader, "normalModel"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void TriangleGeometry::update()
{
}