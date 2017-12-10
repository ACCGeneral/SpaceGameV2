#pragma once

#include "action.h"
#include "component.h"


class fleeaction : public action
{
private:

	std::shared_ptr<ACC::entity> fleefrom;

	std::shared_ptr<AIcomp> myai;
	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<transposecomponent> fleetrans;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<physics> myphyscomp;
	std::shared_ptr<collisioncomp> mycol;

public:

	fleeaction()
	{
		mytype = "fleeaction";
	}

	void setflee(std::shared_ptr<ACC::entity> flee);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};