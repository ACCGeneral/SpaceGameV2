#pragma once

#include "action.h"
#include "component.h"


class fleeaction : public action
{
private:

	std::shared_ptr<ACC::entity> fleefrom;

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