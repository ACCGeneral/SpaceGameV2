#pragma once

#include "action.h"
#include "component.h"
#include "Maneuvers.h"

class manaction : public action
{

private:

	maneuver myman;
	std::shared_ptr<ACC::entity> fleefrom;
	int mannum;
	int roll; 

public:

	manaction()
	{
		mytype = "manaction";
	}

	void setflee(std::shared_ptr<ACC::entity> flee);
	void setman(maneuver mym);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};