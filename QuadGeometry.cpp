#include "QuadGeometry.h"

QuadGeometry::QuadGeometry(const std::string objFileName, Texture* newTexture)
{
	texture = newTexture;

	// ------------------------- LOAD GEOMETRY FROM FILE -------------------------
	std::ifstream objFile(objFileName);
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec3> vertexNormals;

	std::vector<int> vertexIndices;
	std::vector<int> textureCoordinateIndices;
	std::vector<int> vertexNormalIndices;

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

				vertexes.push_back(point);
			}
			else if (label == "vt") {
				glm::vec2 textureCoordinate;
				ss >> textureCoordinate.x >> textureCoordinate.y;

				textureCoordinates.push_back(textureCoordinate);
			}
			else if (label == "vn") {
				glm::vec3 vertexNormal;
				ss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;

				// Normalize all the normals
				vertexNormals.push_back(glm::normalize(vertexNormal));
			}
			else if (label == "f") {
				std::string faceVertex;

				// Iterate over the 4 groups of indices in the face line
				for (int i = 0; i < 4; i++) {
					ss >> faceVertex;
					pushFaceIndices(faceVertex, vertexIndices, textureCoordinateIndices, vertexNormalIndices);
				}
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFileName << std::endl;
	}

	objFile.close();

	// Process the data based on the indices collected
	std::vector<glm::vec3> realVertexes;
	std::vector<glm::vec2> realTextureCoordinates;
	std::vector<glm::vec3> realVertexNormals;

	for (int i = 0; i < vertexIndices.size(); i++) {
		realVertexes.push_back(vertexes[vertexIndices[i]]);
	}	
	for (int i = 0; i < vertexIndices.size(); i++) {
		realTextureCoordinates.push_back(textureCoordinates[textureCoordinateIndices[i]]);
	}
	for (int i = 0; i < vertexIndices.size(); i++) {
		realVertexNormals.push_back(vertexNormals[vertexNormalIndices[i]]);
	}

	numVertices = realVertexes.size();

	// ------------------------- FINISHED LOADING -------------------------

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, &VBO[0]);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// VERTEX

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* realVertexes.size(), realVertexes.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// TEXTURE

	// Bind VBO to the bound VAO, and store the texture coordinate data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* realTextureCoordinates.size(), realTextureCoordinates.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass the texture coordinate data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	// NORMAL

	// Bind VBO to the bound VAO, and store the vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * realVertexNormals.size(), realVertexNormals.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 2 to pass the vertex normal data through to the shader
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

QuadGeometry::~QuadGeometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(3, &VBO[0]);
	glDeleteVertexArrays(1, &VAO);
}

void QuadGeometry::draw(GLuint shader, const glm::mat4& C)
{
	if (materials) {
		materials->sendMatToShader(shader);
	}

	if (texture) {
		texture->sendTextureToShader(shader);
	}

	glUseProgram(shader);

	glm::highp_mat3 normalMatrix = glm::highp_mat3(glm::transpose(glm::inverse(C)));

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));
	glUniformMatrix3fv(glGetUniformLocation(shader, "normalModel"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using quads
	glDrawArrays(GL_QUADS, 0, numVertices);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void QuadGeometry::update(const glm::mat4& C)
{
}

void QuadGeometry::pushFaceIndices(std::string faceVertex, std::vector<int>& vertexIndices, std::vector<int>& textureCoordinateIndices, std::vector<int>& vertexNormalIndices) {
	// Vertex index
	int delimiterIdx = faceVertex.find("/");
	vertexIndices.push_back(std::stoi(faceVertex.substr(0, delimiterIdx)) - 1);
	faceVertex = faceVertex.substr(delimiterIdx + 1);

	// Texture index
	delimiterIdx = faceVertex.find("/");
	textureCoordinateIndices.push_back(std::stoi(faceVertex.substr(0, delimiterIdx)) - 1);
	faceVertex = faceVertex.substr(delimiterIdx + 1);

	// Normal index
	vertexNormalIndices.push_back(std::stoi(faceVertex) - 1);
}