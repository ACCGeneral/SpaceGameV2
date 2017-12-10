#pragma once

#include "action.h"


class attackaction : public action
{

private:

	std::shared_ptr<ACC::entity> mytarget;
	std::shared_ptr<teamunits> myteam;
	float orgmaxvel;

	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<collisioncomp> mycol;
	std::shared_ptr<physics> myphyscomp;
	std::shared_ptr<fighterguns> myguns;
	std::shared_ptr<AIcomp> myai;

	std::shared_ptr<physics> enemyphys;
	std::shared_ptr<collisioncomp> enemycol;
	std::shared_ptr<transposecomponent> attacktrans;

public:

	attackaction()
	{
		mytype = "attackaction";
	}
	void setteam(std::shared_ptr<teamunits> team);
	void gettarget(std::shared_ptr<ACC::entity> tar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};