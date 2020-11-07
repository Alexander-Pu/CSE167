#include "TriangleFacedModel.h"

TriangleFacedModel::TriangleFacedModel(std::vector<glm::vec3> vertexes, std::vector<glm::vec3> vertexNormals, std::vector<glm::uvec3> vertexIndexes, std::vector<glm::uvec3> vertexNormalIndexes)
	: vertexes(vertexes)
	, vertexNormals(vertexNormals)
	, vertexIndexes(vertexIndexes)
	, vertexNormalIndexes(vertexNormalIndexes)
	, scale(1.0f)
	, rotation(glm::mat4(1))
{
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Set the color. 
	color = glm::vec3(1.0f, 0.0f, 0.0f);

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexes.size(), vertexes.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(2, &EBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * vertexIndexes.size(), vertexIndexes.data(), GL_STATIC_DRAW);

	// Bind VBO to the bound VAO, and store the vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals.size(), vertexNormals.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass the vertex normal data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	// Generate EBO, bind the EBO to the bound VAO, and send the vertex normal index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * vertexNormalIndexes.size(), vertexNormalIndexes.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TriangleFacedModel::~TriangleFacedModel() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(2, &EBO[0]);
	glDeleteVertexArrays(1, &VAO);
}

void TriangleFacedModel::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Rotate model
	glm::highp_mat4 rotatedModel = model * rotation;

	// Compute scaled model
	glm::highp_mat4 scaledModel = glm::scale(glm::vec3(scale)) * rotatedModel;

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(scaledModel));
	//glUniformMatrix4fv(glGetUniformLocation(shader, "normalTranslationMatrix"), 1, GL_FALSE, glm::value_ptr(TriangleFacedModel::normalTransformationMatrix));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, 3 * vertexIndexes.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void TriangleFacedModel::update()
{
}

void TriangleFacedModel::scaleObject(double delta)
{
	scale += delta;

	// Set minimum size
	if (scale <= 0.1f) {
		scale = 0.1f;
	}
}

// Rotates the object based on world space
void TriangleFacedModel::rotateObject(GLfloat radians, glm::vec3 axis)
{
	rotation = glm::rotate(radians, axis) * rotation;
}

void TriangleFacedModel::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
