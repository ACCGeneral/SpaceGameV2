#pragma once

#include "action.h"
#include "component.h"
#include "Maneuvers.h"

class bombingrunaction : public action
{

private:

	std::shared_ptr<ACC::entity> mytarget;
	maneuver myman;

	bool man;
	int mannum;
	int roll;


	std::shared_ptr<AIcomp> myai;
	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<physics> myphys;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<collisioncomp> mycol;
	std::shared_ptr<fighterguns> myguns;

	std::shared_ptr<transposecomponent> enemytrans;
	std::shared_ptr<collisioncomp> enemycol;

public:

	bombingrunaction()
	{
		mytype = "bombingrunaction";
	}

	void setmyman(maneuver mym);
	void setcaptarget(std::shared_ptr<ACC::entity> captar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};