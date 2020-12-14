#ifndef _ASTRONAUT_HANDLER_H_
#define _ASTRONAUT_HANDLER_H_

#include "Astronaut.h"
#include "AstroAI.h"
#include "Collider.h"
#include "Time.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include "irrKlang.h"

#include <cstdlib>
#include <utility>
#include <map>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;

class AstronautHandler
{
private:
	ParticleSystem* particleSystem;
	ISoundEngine* soundEngine;
	std::map<AstroAI*, bool> astronautToActive;

	static glm::vec3 hexToRGB(int hex);
	static glm::vec3 getSpawnPoint();
	void handleSpawnParticles(Astronaut* astronaut);
	void handleDespawnParticles(Astronaut* astronaut);
public:
	AstronautHandler(ParticleSystem* particleSystem, ISoundEngine* soundEngine, std::vector<Geometry*>& idle, std::vector<Geometry*>& walking);
	~AstronautHandler();
	std::vector<Astronaut*> getAstronauts();
	void updateActive();
	void handleCollisions(std::vector<std::pair<Collider*, Collider*>> collidingColliders);
	void handleActivate();
	void handleDeactivate();
};

#endif
