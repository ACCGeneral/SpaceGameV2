#pragma once

#include "action.h"
#include "component.h"


class toenemycapship : public action
{
private:
	std::shared_ptr<ACC::entity> capattack;

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