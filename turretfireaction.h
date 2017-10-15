#pragma once

#include "action.h"
#include "component.h"

class turretfireaction : public action
{

private:

	std::shared_ptr<ACC::entity> target;
	std::shared_ptr<ACC::entity> turretguns;

	void firegunstest(float dt);

public:

	turretfireaction()
	{
		mytype = "turretfireaction";
	}

	void settarget(std::shared_ptr<ACC::entity> tar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	
	void end();
};