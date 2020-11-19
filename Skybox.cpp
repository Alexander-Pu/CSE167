#include "Skybox.h"

Skybox::Skybox(vector<std::string> textureFaceFiles, GLfloat scale)
	: scale(glm::scale(glm::vec3(scale)))
	, rotation(glm::mat4(1))
	, location(glm::vec3(0))
{
	// Set the model matrix to the identity matrix. 
	model = glm::mat4(1);

	/*
	 * Cube indices used below.
	 *    4----7
	 *   /|   /|
	 *  0-+--3 |
	 *  | 5--+-6
	 *  |/   |/
	 *  1----2
	 *
	 */

	 // The 8 vertices of a cube.
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		glm::ivec3(0, 2, 1),
		glm::ivec3(2, 0, 3),
		// Back face.
		glm::ivec3(7, 5, 6),
		glm::ivec3(5, 7, 4),
		// Right face.
		glm::ivec3(3, 6, 2),
		glm::ivec3(6, 3, 7),
		// Left face.
		glm::ivec3(4, 1, 5),
		glm::ivec3(1, 4, 0),
		// Top face.
		glm::ivec3(4, 3, 0),
		glm::ivec3(3, 4, 7),
		// Bottom face.
		glm::ivec3(1, 6, 5),
		glm::ivec3(6, 1, 2),
	};

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Load cube map texture

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

Skybox::~Skybox() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &TEXTURE);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Rotate model
	glm::highp_mat4 rotatedModel = rotation * model;

	// Compute scaled model
	glm::highp_mat4 scaledModel = scale * rotatedModel;

	// Translate the model
	glm::highp_mat4 translatedModel = glm::translate(location) * scaledModel;

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(translatedModel));

	// Draw in the back of the scene
	glDepthMask(GL_FALSE);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind textures
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, 3 * 8, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Skybox::update() {

}