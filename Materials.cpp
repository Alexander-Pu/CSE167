#include "Materials.h"

Materials::Materials(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	: ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
	, shininess(shininess)
{
}

Materials::~Materials()
{
}

// Assumes this is being called in a method that is already calling glUseProgram()
void Materials::sendMatToShader(GLuint shaderID) {
	glUseProgram(shaderID);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shaderID, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shaderID, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shaderID, "specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shaderID, "shininess"), shininess);

	std::cout << "Sending materials" << shaderID << ", " << glm::to_string(ambient) << ", " << glm::to_string(diffuse) << ", " << glm::to_string(specular) << ", " << shininess << std::endl;

	glUseProgram(0);
}
