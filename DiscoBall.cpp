#include "DiscoBall.h"

DiscoBall::DiscoBall(int numFacesVertical, int numFacesHorizontal)
{
	// Creating the faces based on numFacesVertical and numFacesHorizontal
	int i, j;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> normals;
	int indicator = 0;
	double numFacesVerticalDouble = (double) numFacesVertical;
	double numFacesHorizontalDouble = (double) numFacesHorizontal;

	float discoOffset = glm::pi<double>() / numFacesHorizontalDouble;
	bool useOffset = false;

	for (i = 0; i < numFacesVertical; i++) {
		double topTheta = glm::pi<double>() * (double) i / numFacesVerticalDouble;
		double topZ = glm::cos(topTheta);
		double topInverseZ = glm::sin(topTheta);

		double bottomTheta = glm::pi<double>() * (double) (i + 1.0) / numFacesVerticalDouble;
		double bottomZ = glm::cos(bottomTheta);
		double bottomInverseZ = glm::sin(bottomTheta);

		float offsetToUse = useOffset ? discoOffset : 0;
		useOffset = !useOffset;
		for (j = 0; j < numFacesHorizontal; j++) {
			double leftPhi = glm::two_pi<double>() * (double) j / numFacesHorizontalDouble + offsetToUse;
			double rightPhi = glm::two_pi<double>() * (double) (j + 1.0) / numFacesHorizontalDouble + offsetToUse;

			double leftX = glm::cos(leftPhi);
			double leftY = glm::sin(leftPhi);
			double rightX = glm::cos(rightPhi);
			double rightY = glm::sin(rightPhi);

			// Adding vertices in counter clockwise order

			// Add top left vertex
			double topLeftX = leftX * topInverseZ;
			double topLeftY = leftY * topInverseZ;
			vertices.push_back(topLeftX);
			vertices.push_back(topLeftY);
			vertices.push_back(topZ);
			indices.push_back(indicator);
			indicator++;

			// Add bottom left vertex
			double bottomLeftX = leftX * bottomInverseZ;
			double bottomLeftY = leftY * bottomInverseZ;
			vertices.push_back(bottomLeftX);
			vertices.push_back(bottomLeftY);
			vertices.push_back(bottomZ);
			indices.push_back(indicator);
			indicator++;

			// Add bottom right vertex
			double bottomRightX = rightX * bottomInverseZ;
			double bottomRightY = rightY * bottomInverseZ;
			vertices.push_back(bottomRightX);
			vertices.push_back(bottomRightY);
			vertices.push_back(bottomZ);
			indices.push_back(indicator);
			indicator++;

			// Add top right vertex
			double topRightX = rightX * topInverseZ;
			double topRightY = rightY * topInverseZ;
			vertices.push_back(topRightX);
			vertices.push_back(topRightY);
			vertices.push_back(topZ);
			indices.push_back(indicator);
			indicator++;

			float xAvg = (topLeftX + bottomLeftX + bottomRightX + topRightX) / 4.0;
			float yAvg = (topLeftY + bottomLeftY + bottomRightY + topRightY) / 4.0;
			float zAvg = (topZ + bottomZ) / 2.0;

			// Same normals for all corners of the face
			normals.push_back(xAvg);
			normals.push_back(yAvg);
			normals.push_back(zAvg);

			normals.push_back(xAvg);
			normals.push_back(yAvg);
			normals.push_back(zAvg);

			normals.push_back(xAvg);
			normals.push_back(yAvg);
			normals.push_back(zAvg);

			normals.push_back(xAvg);
			normals.push_back(yAvg);
			normals.push_back(zAvg);
		}
	}

	numVertices = indices.size();

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass the vertex normal data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

DiscoBall::~DiscoBall()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void DiscoBall::draw(GLuint shader, const glm::mat4& C)
{
	glUseProgram(shader);

	glm::highp_mat3 normalMatrix = glm::highp_mat3(glm::transpose(glm::inverse(C)));

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));
	glUniformMatrix3fv(glGetUniformLocation(shader, "normalModel"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the faces of the sphere using quads, indexed with the EBO
	glDrawElements(GL_QUADS, numVertices, GL_UNSIGNED_INT, NULL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void DiscoBall::update(const glm::mat4& C)
{
}