#pragma once

#include "System.h"
#include <vector>

class OctTree;

class CollisionResolveSystem : public ACC::System
{

private:

	sound physicssound;

public:

	CollisionResolveSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);

	void cleanup() {}
};