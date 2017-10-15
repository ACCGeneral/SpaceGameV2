#pragma once

#include "Teamunits.h"
#include "action.h"
#include "Maneuvers.h"
#include "utilityfunction.h"
#include "DecisionFunctions.h"


class AImanager
{
private:

	std::shared_ptr<teamunits> goodguys;
	std::shared_ptr<teamunits> badguys;

	void setfreecheck();
	void decideonaction(std::shared_ptr<ACC::entity> unit);

	maneuvers mymanuvers;

public:

	AImanager();
	void addtoteamone(std::shared_ptr<ACC::entity> toadd);
	void addtoteamttwo(std::shared_ptr<ACC::entity> toadd);
	void update(float dt, std::shared_ptr<world> myworld);
	void removefromlist(std::shared_ptr<ACC::entity> toremove);
	void addenemycap(std::shared_ptr<ACC::entity> enecap)
	{
		badguys->allunits.push_back(enecap);
		badguys->capital = enecap;
	}
	void addallycap(std::shared_ptr<ACC::entity> allcap)
	{
		goodguys->allunits.push_back(allcap);
		goodguys->capital = allcap;
	}

};
