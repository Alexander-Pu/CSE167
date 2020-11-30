#include "Skybox.h"

Skybox::Skybox()
{
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

	numIndices = 3 * indices.size();

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO[0]);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Skybox::~Skybox() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Skybox::draw(GLuint shader, const glm::mat4& C)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));

	// Draw in the back of the scene
	glDepthMask(GL_FALSE);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Skybox::update() {
}