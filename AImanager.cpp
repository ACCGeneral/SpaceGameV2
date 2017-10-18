#include "AImanager.h"


AImanager::AImanager()
{
	goodguys = std::make_shared<teamunits>();
	badguys = std::make_shared<teamunits>();
}

void AImanager::addtoteamone(std::shared_ptr<ACC::entity> toadd)
{
	goodguys->allunits.push_back(toadd);

	if (toadd->returntype() == enttypes::Player)
	{
		goodguys->player = toadd;
	}
	else if (toadd->returntype() == enttypes::Fighter)
	{
		goodguys->fighters.push_back(toadd);
	}
	else if (toadd->returntype() == enttypes::Bomber)
	{
		goodguys->bomber.push_back(toadd);
	}
	else if (toadd->returntype() == enttypes::Turrent)
	{
		goodguys->turrets.push_back(toadd);
	}
	
	

}
void AImanager::addtoteamttwo(std::shared_ptr<ACC::entity> toadd)
{
	badguys->allunits.push_back(toadd);

	if (toadd->returntype() == enttypes::Fighter)
	{
		badguys->fighters.push_back(toadd);
	}
	else if (toadd->returntype() == enttypes::Bomber)
	{
		badguys->bomber.push_back(toadd);
	}
	else if (toadd->returntype() == enttypes::Turrent)
	{
		badguys->turrets.push_back(toadd);
	}

}

void AImanager::update(float dt, std::shared_ptr<world> myworld)
{
	for (int i = 0; i < goodguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = goodguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp != NULL && myaicomp->free == true)
		{
			decideonaction(goodguys->allunits[i]);
			myaicomp->free = false;
		}
	}
	for (int i = 0; i < badguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = badguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp->free == true)
		{
			decideonaction(badguys->allunits[i]);
			myaicomp->free = false;
		}	
	}

	
	for (int i = 0; i < goodguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = goodguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp && myaicomp->myaction)
		{
			myaicomp->myaction->run(dt, myworld);
		}
		
	}
	for (int i = 0; i <  badguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = badguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp->myaction)
		{
			myaicomp->myaction->run(dt, myworld);
		}
	}
	
	setfreecheck();

}

void AImanager::removefromlist(std::shared_ptr<ACC::entity> toremove)
{
	std::shared_ptr<teamcomponent> teamcomp = toremove->getcomponent<teamcomponent>();

	if (teamcomp->ally)
	{
		goodguys->removeunit(toremove);
	}
	else
	{
		badguys->removeunit(toremove);
	}

}

void AImanager::setfreecheck()
{
	for (int i = 0; i < goodguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = goodguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp && myaicomp->myaction && myaicomp->myaction->returntime() <= 0)
		{
			myaicomp->myaction->end();
			myaicomp->myaction = NULL;
			myaicomp->score = 0;
			myaicomp->free = true;
		}
	}
	for (int i = 0; i < badguys->allunits.size(); i++)
	{
		std::shared_ptr<AIcomp> myaicomp = badguys->allunits[i]->getcomponent<AIcomp>();
		if (myaicomp->myaction && myaicomp->myaction->returntime() <= 0)
		{
			myaicomp->myaction->end();
			myaicomp->myaction = NULL;
			myaicomp->score = 0;
			myaicomp->free = true;
		}
	}
	

}

void AImanager::decideonaction(std::shared_ptr<ACC::entity> unit)
{

	std::shared_ptr<teamcomponent> teamcomp = unit->getcomponent<teamcomponent>();
	std::shared_ptr<teamunits> myteam;
	std::shared_ptr<teamunits> enemyteam;
	if (teamcomp->ally == false)
	{
		myteam = badguys;
		enemyteam = goodguys;
	}
	else
	{
		myteam = goodguys;
		enemyteam = badguys;
	}

	if (unit->returntype() == enttypes::Fighter)
	{
		toenemycap(unit,badguys,500.0f);
		attackflyer(unit, enemyteam, myteam, mymanuvers.getrandom());
		attackrun(unit, enemyteam, mymanuvers.getrandom(), mymanuvers.getcurtain("attackrun"));
	}
	else if (unit->returntype() == enttypes::Bomber)
	{
		toenemycap(unit, enemyteam, 500.0f);
		bombingrun(unit, enemyteam, mymanuvers.getrandom(), mymanuvers.getcurtain("attackrun"));
		attackturret(unit, enemyteam, mymanuvers.getrandom());
	}
	else if (unit->returntype() == enttypes::Turrent)
	{
		turrettarget(unit, enemyteam);
		turretidel(unit, enemyteam, 600.0f);
	}

}


