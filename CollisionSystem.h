#pragma once

#include "System.h"
#include <vector>

class CollisionSystem : public ACC::System
{

private:

	std::vector<std::shared_ptr<ACC::entity>> collisionEnts;
	sound physicssound;

public:

	CollisionSystem(std::shared_ptr<world> w);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	void dealwithcols();

	void healthanddamage(std::shared_ptr<ACC::entity> entone, std::shared_ptr<ACC::entity> enttwo);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);


};