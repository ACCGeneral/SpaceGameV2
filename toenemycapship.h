#pragma once

#include "action.h"
#include "component.h"


class toenemycapship : public action
{
private:
	std::shared_ptr<ACC::entity> capattack;

	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<physics> myphys;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<AIcomp> myai;
	std::shared_ptr<collisioncomp> mycol;
	std::shared_ptr<transposecomponent> captrans;

public:

	toenemycapship()
	{
		mytype = "toenemycapship";
	}

	void setcaptarget(std::shared_ptr<ACC::entity> captar);

	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};