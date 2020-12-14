#include "AstronautHandler.h"

AstronautHandler::AstronautHandler(ParticleSystem* particleSystem, std::vector<Geometry*>& idle, std::vector<Geometry*>& walking)
	: particleSystem(particleSystem)
{
	Astronaut* blackAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x3E474E));
	AstroAI* blackAI = new AstroAI(blackAstronaut);
	Astronaut* blueAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x132ED1));
	AstroAI* blueAI = new AstroAI(blueAstronaut);
	Astronaut* brownAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x71491D));
	AstroAI* brownAI = new AstroAI(brownAstronaut);
	Astronaut* cyanAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x39FEDD));
	AstroAI* cyanAI = new AstroAI(cyanAstronaut);
	Astronaut* darkGreenAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x13802C));
	AstroAI* darkGreenAI = new AstroAI(darkGreenAstronaut);
	Astronaut* limeAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x4EEF38));
	AstroAI* limeAI = new AstroAI(limeAstronaut);
	Astronaut* orangeAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0xF17D0C));
	AstroAI* orangeAI = new AstroAI(orangeAstronaut);
	Astronaut* pinkAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0xEC54BB));
	AstroAI* pinkAI = new AstroAI(pinkAstronaut);
	Astronaut* purpleAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0x6C2FBC));
	AstroAI* purpleAI = new AstroAI(purpleAstronaut);
	Astronaut* whiteAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0xD6DFF1));
	AstroAI* whiteAI = new AstroAI(whiteAstronaut);
	Astronaut* yellowAstronaut = new Astronaut(getSpawnPoint(), idle, walking, hexToRGB(0xF6F657));
	AstroAI* yellowAI = new AstroAI(yellowAstronaut);
	astronautToActive[blackAI] = false;
	astronautToActive[blueAI] = false;
	astronautToActive[brownAI] = false;
	astronautToActive[cyanAI] = false;
	astronautToActive[darkGreenAI] = false;
	astronautToActive[limeAI] = false;
	astronautToActive[orangeAI] = false;
	astronautToActive[pinkAI] = false;
	astronautToActive[purpleAI] = false;
	astronautToActive[whiteAI] = false;
	astronautToActive[yellowAI] = false;
}

AstronautHandler::~AstronautHandler()
{
}


glm::vec3 AstronautHandler::hexToRGB(int hex) {
	glm::vec3 rgb;
	rgb.r = ((hex >> 16) & 0xFF) / 255.0;
	rgb.g = ((hex >> 8) & 0xFF) / 255.0;
	rgb.b = ((hex) & 0xFF) / 255.0;

	return rgb;
}

glm::vec3 AstronautHandler::getSpawnPoint() {
	int randX = rand() % (15 - (-15)) - 15;
	int randZ = rand() % (20 - 2) + 2;

	return glm::vec3(randX, 300, randZ);
}

std::vector<Astronaut*> AstronautHandler::getAstronauts() {
	std::vector<Astronaut*> astronauts;
	for (auto const& entry : astronautToActive) {
		AstroAI* ai = entry.first;
		astronauts.push_back(ai->getAstronaut());
	}

	return astronauts;
}

void AstronautHandler::updateActive() {
	for (auto const& entry : astronautToActive) {
		if (!entry.second) {
			continue;
		}

		AstroAI* ai = entry.first;
		ai->update();
	}
}

void AstronautHandler::handleCollisions(std::vector<std::pair<Collider*, Collider*>> collidingColliders) {
	for (auto const& entry : astronautToActive) {
		if (!entry.second) {
			continue;
		}

		AstroAI* ai = entry.first;
		ai->handleCollisions(collidingColliders);
	}
}

void AstronautHandler::handleActivate() {
	int numActive = 0;
	std::vector<AstroAI*> inactive;
	for (auto const& entry : astronautToActive) {
		if (entry.second) {
			numActive++;
		}
		else {
			inactive.push_back(entry.first);
		}
	}

	if (numActive < 9) {
		if (rand() % 100000 < 2) {
			AstroAI* ai = inactive[rand() % inactive.size()];
			glm::vec3 location = ai->getAstronaut()->getTransform()->getLocation();
			location.y = 0.4;
			ai->getAstronaut()->getTransform()->setLocation(location);
			ai->getAstronaut()->getTransform()->update(glm::mat4(1));
			astronautToActive[ai] = true;
			ai->setState(AIState::SPAWNING);
			handleSpawnParticles(ai->getAstronaut());
		}
	}
}

void AstronautHandler::handleDeactivate() {
	int numActive = 0;
	std::vector<AstroAI*> active;
	for (auto const& entry : astronautToActive) {
		if (entry.second) {
			numActive++;
			active.push_back(entry.first);
		}
	}

	if (numActive > 0) {
		if (rand() % 1000000 < 2) {
			AstroAI* ai = active[rand() % active.size()];

			// Don't despawn astronauts that are spawning in
			if (ai->getState() == AIState::SPAWNING) {
				return;
			}

			glm::vec3 location = ai->getAstronaut()->getTransform()->getLocation();
			location.y = 300;
			ai->getAstronaut()->getTransform()->setLocation(location);
			astronautToActive[ai] = false;
			handleDespawnParticles(ai->getAstronaut());
		}
	}
}

void AstronautHandler::handleSpawnParticles(Astronaut* astronaut) {
	for (int i = 0; i < 100; i++) {
		glm::vec3 particleVelocity = glm::normalize(glm::vec3(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500));
		glm::vec3 location = astronaut->getTransform()->getLocation();
		location.y = 3;
		float lifeSpan = 3;
		particleSystem->addParticle(location + lifeSpan * particleVelocity, -particleVelocity, [](glm::vec3 x) {return x; }, lifeSpan, glm::vec3(1), astronaut->getColor());
	}
}

velocity_alterer alterer = [](glm::vec3 velocity) {
	glm::vec3 newVelocity = velocity;
	newVelocity.x += float((rand() % 1000 - 500.0)) / 2000.0;
	newVelocity.y += float((rand() % 1000 - 500.0)) / 2000.0;
	newVelocity.z += float((rand() % 1000 - 500.0)) / 2000.0;
	return newVelocity;
};

void AstronautHandler::handleDespawnParticles(Astronaut* astronaut) {
	for (int i = 0; i < 100; i++) {
		glm::vec3 particleVelocity = 3.0f * glm::vec3(0, 1, 0);
		glm::vec3 location = astronaut->getTransform()->getLocation();
		location.y = 2;
		float lifeSpan = .8;
		particleSystem->addParticle(location, particleVelocity, alterer, lifeSpan, astronaut->getColor(), glm::vec3(1, 0, 0));
	}
}