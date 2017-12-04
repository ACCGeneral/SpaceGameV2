#pragma once

#include "System.h"
#include <vector>

class TargetingSystem : public ACC::System
{

private:

	std::vector<std::shared_ptr<ACC::entity>> targets;
	std::shared_ptr<ACC::entity> player;
	std::shared_ptr<ACC::entity> trackerarrows;

public:

	TargetingSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	void trackerarrowupdate();

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);

	void cleanup() {}

};