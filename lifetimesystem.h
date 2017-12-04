#pragma once

#include "System.h"
#include <vector>

class lifetimesystem : public ACC::System
{

private:

	std::vector<std::shared_ptr<ACC::entity>> lifeents;

public:

	lifetimesystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);

	void cleanup() {}

};