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

	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<transposecomponent> enemycapsing;
	std::shared_ptr<AIcomp> myai;
	std::shared_ptr<physics> myphys;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<collisioncomp> mycol;
	std::shared_ptr<collisioncomp> enemycol;
	std::shared_ptr<fighterguns> myguns;

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