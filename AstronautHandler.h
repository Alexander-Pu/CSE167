#ifndef _ASTRONAUT_HANDLER_H_
#define _ASTRONAUT_HANDLER_H_

#include "Astronaut.h"
#include "AstroAI.h"
#include "Collider.h"
#include "Time.h"
#include "Animation.h"

#include <cstdlib>
#include <utility>
#include <map>

class AstronautHandler
{
private:
	std::map<AstroAI*, bool> astronautToActive;

	static glm::vec3 hexToRGB(int hex);
	static glm::vec3 getSpawnPoint();
public:
	AstronautHandler(std::vector<Geometry*>& idle, std::vector<Geometry*>& walking);
	~AstronautHandler();
	std::vector<Astronaut*> getAstronauts();
	void updateActive();
	void handleCollisions(std::vector<std::pair<Collider*, Collider*>> collidingColliders);
	void handleActivate();
	void handleDeactivate();
};

#endif
