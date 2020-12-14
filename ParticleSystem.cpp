#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxNumParticles)
	: maxNumParticles(maxNumParticles)
	, particleIndex(0)
{
	std::vector<glm::vec3> locations;
	for (int i = 0; i < maxNumParticles; i++) {
		locations.push_back(glm::vec3(0, 300, 0));
	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);
	// Bind VAO
	glBindVertexArray(VAO);

	// LOCATION

	// Bind VBO to the bound VAO, and store the location data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * locations.size(), locations.data(), GL_DYNAMIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// COLOR
	std::vector<glm::vec3> colors;
	for (int i = 0; i < maxNumParticles; i++) {
		colors.push_back(glm::vec3(1, 0, 0));
	}

	// Bind VBO to the bound VAO, and store the color data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);

	// Enable Vertex Attribute 1 to pass the texture coordinate data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Init particles
	for (int i = 0; i < maxNumParticles; i++) {
		addParticle(glm::vec3(0, 300, 0), glm::vec3(0, 1, 0), [](glm::vec3 x) {return x; }, 1, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	}
}

ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, &VBO[0]);
	glDeleteVertexArrays(1, &VAO);

	for (auto particle : particles) {
		delete particle;
	}
}

void ParticleSystem::addParticle(glm::vec3 origin, glm::vec3 velocity, velocity_alterer velocityAlterer, float lifeSpan, glm::vec3 initColor, glm::vec3 endColor)
{
	if (particleIndex >= maxNumParticles) {
		particleIndex = 0;
	}
	if (particles.size() != maxNumParticles) {
		particles.push_back(new Particle(origin, velocity, velocityAlterer, lifeSpan, initColor, endColor));
	}
	else {
		particles[particleIndex]->alter(origin, velocity, velocityAlterer, lifeSpan, initColor, endColor);
	}
	particleIndex++;
}

void ParticleSystem::update()
{
	for (Particle* particle : particles) {
		particle->update();
	}
}

void ParticleSystem::draw(GLuint shader)
{
	std::vector<glm::vec3> locations;
	std::vector<glm::vec3> colors;
	for (auto particle : particles) {
		locations.push_back(particle->getLocation());
		colors.push_back(particle->getColor());
	}

	// Activate the shader program 
	glUseProgram(shader);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the location data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * locations.size(), locations.data());

	// Bind VBO to the bound VAO, and store the color data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * colors.size(), colors.data());

	// Draw the particles 
	glDrawArrays(GL_POINTS, 0, maxNumParticles);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}
