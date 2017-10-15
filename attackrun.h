#pragma once

#include "action.h"
#include "Maneuvers.h"

class attackrunaction : public action
{

private:

	std::shared_ptr<ACC::entity> mytarget;
	maneuver myman;
	maneuver attackman;
	maneuver activeman;

	int mannum;
	int roll;

public:

	attackrunaction()
	{
		mytype = "attackrunaction";
	}

	void setmyman(maneuver mym);
	void setattackman(maneuver mym);
	void gettarget(std::shared_ptr<ACC::entity> tar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};