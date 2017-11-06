#pragma once

#include "action.h"
#include "component.h"
#include "Maneuvers.h"

class TurretAttack : public action
{

private:

	std::shared_ptr<ACC::entity> mytarget;
	maneuver myman;

	bool man;
	int mannum;
	int roll;

public:

	TurretAttack()
	{
		mytype = "TurretAttack";
	}
	void setmyman(maneuver mym);
	void setcaptarget(std::shared_ptr<ACC::entity> captar);
	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};