#pragma once

#include "action.h"
#include "component.h"

class turretfireaction : public action
{

private:

	std::shared_ptr<ACC::entity> target;
	std::shared_ptr<ACC::entity> turretguns;

	void firegunstest(float dt);

	std::shared_ptr<directioncomponent> mydir;
	std::shared_ptr<turretcomp> myturretcomp;
	std::shared_ptr<transposecomponent> mytrans;
	std::shared_ptr<animationholdercomp> myanim;
	std::shared_ptr<directioncomponent> baseturretdir;

	std::shared_ptr<transposecomponent> mygunstrans;
	std::shared_ptr<directioncomponent> gundir;
	std::shared_ptr<animationholdercomp> gunsanim;
	std::shared_ptr<fighterguns> myguns;
	

	std::shared_ptr<collisioncomp> enemycol;
	std::shared_ptr<transposecomponent> targettrans;
	
public:

	turretfireaction()
	{
		mytype = "turretfireaction";
	}

	void settarget(std::shared_ptr<ACC::entity> tar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	
	void end();
};