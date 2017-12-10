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

	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<physics> myphys;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<AIcomp> myai;
	std::shared_ptr<collisioncomp> mycol;

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