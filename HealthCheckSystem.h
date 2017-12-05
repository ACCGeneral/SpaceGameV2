#pragma once

#include "System.h"
#include <vector>

class HealthCheckSystem : public ACC::System
{

private:

	std::vector<std::shared_ptr<ACC::entity>> healthobjects;

public:

	HealthCheckSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys);

	void damageevents();

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);

	void createondeathevent(int ID);

	void cleanup() {}


};