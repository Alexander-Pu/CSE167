#include "Sphere.h"

Sphere::Sphere(vector<std::string> textureFaceFiles, int numFacesVertical, int numFacesHorizontal)
	: scale(glm::scale(glm::vec3(15)))
	, rotation(glm::rotate(0 * glm::half_pi<GLfloat>(), glm::vec3(1, 0, 0)))
	, location(glm::vec3(0))
{
	model = glm::mat4(1);

	// The color of the sphere.
	color = glm::vec3(1.0f, 0.95f, 0.1f); 

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

	numVertices = vertices.size();

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
	glGenBuffers(2, &EBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Generate EBO, bind the EBO to the bound VAO, and send the vertex normal index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);


	// Bind texture id
	glGenTextures(1, &TEXTURE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE);

	// Load separate texture faces
	int width, height, numChannels;
	unsigned char* textureData;
	for (unsigned int i = 0; i < textureFaceFiles.size(); i++) {
		textureData = stbi_load(textureFaceFiles[i].c_str(), &width, &height, &numChannels, 0);
		if (textureData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}
		else {
			std::cerr << "Cubemap texture failed to load at path: " << textureFaceFiles[i] << std::endl;
		}
		stbi_image_free(textureData);
	}
	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sphere::~Sphere()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(2, &EBO[0]);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &TEXTURE);
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Rotate model
	glm::highp_mat4 rotatedModel = rotation * model;

	// Compute scaled model
	glm::highp_mat4 scaledModel = scale * rotatedModel;

	// Translate the model
	glm::highp_mat4 translatedModel = glm::translate(location) * scaledModel;

	// Generate separate normal model
	glm::highp_mat3 normalModel = glm::highp_mat3(glm::transpose(glm::inverse(translatedModel)));

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(translatedModel));
	glUniformMatrix3fv(glGetUniformLocation(shader, "normalModel"), 1, GL_FALSE, glm::value_ptr(normalModel));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind textures
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE);

	// Draw the faces of the sphere using quads, indexed with the EBO
	glDrawElements(GL_QUADS, numVertices, GL_UNSIGNED_INT, NULL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sphere::update()
{
	//model = glm::rotate(.01f, glm::vec3(0, 0, 1)) * model;
}