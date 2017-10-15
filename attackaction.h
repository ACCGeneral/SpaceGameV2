#pragma once

#include "action.h"
class teamunits;

class attackaction : public action
{

private:

	std::shared_ptr<ACC::entity> mytarget;
	std::shared_ptr<teamunits> myteam;
	float orgmaxvel;

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