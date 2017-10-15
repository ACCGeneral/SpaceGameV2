#include "DecisionFunctions.h"

void toenemycap(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, float maxdistance)
{

	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();

	for (int i = 0; i < enemyunits->allunits.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->allunits[i]->getcomponent<transposecomponent>();
		float lenghtto = glm::length(enemytrans->position - mytrans->position);
		if (lenghtto < maxdistance)
		{
			return;
		}
	}

	toenemycapship newaction;
	newaction.settime(1.0f);
	newaction.setcaptarget(enemyunits->capital);
	newaction.addme(me);
	newaction.start();
	myaicomp->myaction = std::make_unique<toenemycapship>(newaction);
	myaicomp->score = 1.0f;

}

void bombingrun(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits>  enemyunits, maneuver touse, maneuver startuse)
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();


	std::shared_ptr<ACC::entity> besttarget;
	float bestscore = 0.0;

	float healthscore;
	float distancescore;
	float anglescore;
	float turretbonus = 1.2;


	for (int i = 0; i < enemyunits->turrets.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->turrets[i]->getcomponent<transposecomponent>();
		std::shared_ptr<healthcomponent> enemyhealth = enemyunits->turrets[i]->getcomponent<healthcomponent>();

		distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
		distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8),0.2,1);

		healthscore = invert(enemyhealth->health/ enemyhealth->maxhp,1);
		healthscore = setclamp(linearandquadratic(0.0f, healthscore, 0.5), 0.5, 1);

		glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
		float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
		float degreecheck = glm::degrees(shotdegrees);

		anglescore = invert(degreecheck / 180.0f,1);
		anglescore = setclamp(logisticscore(2,anglescore,0.6,-10), 0.3, 1);

		float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore) * turretbonus;

		if (newscore > bestscore)
		{
			besttarget = enemyunits->turrets[i];
			bestscore = newscore;
		}
	}

	std::shared_ptr<transposecomponent> enemytrans = enemyunits->capital->getcomponent<transposecomponent>();
	std::shared_ptr<healthcomponent> enemyhealth = enemyunits->capital->getcomponent<healthcomponent>();

	distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
	distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8), 0.2, 1);

	healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
	healthscore = setclamp(linearandquadratic(0.0f, healthscore, 0.5), 0.5, 1);

	glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
	float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
	float degreecheck = glm::degrees(shotdegrees);

	anglescore = invert(degreecheck / 180.0f, 1);
	anglescore = setclamp(logisticscore(2, anglescore, 0.6, -10), 0.3, 1);

	float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);

	if (newscore > bestscore)
	{
		besttarget = enemyunits->capital;
		bestscore = newscore;
	}

	if (myaicomp->score < bestscore)
	{
		myaicomp->score = bestscore;

		bombingrunaction newaction;
		newaction.setcaptarget(besttarget);
		newaction.addme(me);
		newaction.settime(10.0f);
		newaction.setmyman(touse);
		newaction.start();
		myaicomp->myaction = std::make_unique<bombingrunaction>(newaction);
		
	}



}

void attackrun(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, maneuver touse, maneuver startuse)
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();

	std::shared_ptr<ACC::entity> besttarget;
	float bestscore = 0.0;

	float healthscore;
	float distancescore;
	float anglescore;

	std::vector<std::shared_ptr<ACC::entity>> checkents = enemyunits->turrets;
	checkents.push_back(enemyunits->capital);

	for (int i = 0; i < checkents.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = checkents[i]->getcomponent<transposecomponent>();
		std::shared_ptr<healthcomponent> enemyhealth = checkents[i]->getcomponent<healthcomponent>();

		distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
		distancescore = setclamp(linearandquadratic(0.0f, distancescore, 1.4), 0.1, 1);

		healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
		healthscore = setclamp(linearandquadratic(0.0f, healthscore, 1.3), 0.1, 1);

		glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
		float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
		float degreecheck = glm::degrees(shotdegrees);

		anglescore = invert(degreecheck / 180.0f, 1);
		anglescore = setclamp(logisticscore(2, anglescore, 0.7, -10), 0.1, 1);

		float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);

		if (newscore > bestscore)
		{
			besttarget = checkents[i];
			bestscore = newscore;
		}
	}

	if (myaicomp->score < bestscore)
	{
		myaicomp->score = bestscore;

		int i = rand() % 2;

		if (i == 0)
		{
			bombingrunaction newaction;
			newaction.setcaptarget(besttarget);
			newaction.addme(me);
			newaction.settime(20.0f);
			newaction.setmyman(touse);
			newaction.start();
			myaicomp->myaction = std::make_unique<bombingrunaction>(newaction);
		}
		else
		{
			attackrunaction newaction;
			newaction.gettarget(besttarget);
			newaction.addme(me);
			newaction.settime(20.0f);
			newaction.setmyman(touse);
			newaction.setattackman(startuse);
			newaction.start();
			myaicomp->myaction = std::make_unique<attackrunaction>(newaction);
		}

	}

}

void attackflyer(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, std::shared_ptr<teamunits> allyteam, maneuver touse)
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();

	std::shared_ptr<ACC::entity> besttarget;
	float bestscore = 0.0;

	float healthscore;
	float distancescore;
	float anglescore;
	float capshipmulti = 1.0f;



	for (int i = 0; i < enemyunits->bomber.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->bomber[i]->getcomponent<transposecomponent>();
		std::shared_ptr<healthcomponent> enemyhealth = enemyunits->bomber[i]->getcomponent<healthcomponent>();
		std::shared_ptr<AIcomp> enemyai = enemyunits->bomber[i]->getcomponent<AIcomp>();

		distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
		distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8), 0.2, 1);

		healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
		healthscore = setclamp(linearandquadratic(0.0f, healthscore, 0.5), 0.5, 1);

		glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
		float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
		float degreecheck = glm::degrees(shotdegrees);

		anglescore = invert(degreecheck / 180.0f, 1);
		anglescore = setclamp(logisticscore(2, anglescore, 0.6, -10), 0.3, 1);

		float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);


		if (enemyai->myaction != NULL)
		{
			if (enemyai->myaction->returntype() == "fleeaction" || enemyai->myaction->returntype() == "manaction")
			{
				newscore = newscore * 0.6f;
			}
		}

		if (newscore > bestscore)
		{
			besttarget = enemyunits->bomber[i];
			bestscore = newscore;
		}
	}
	for (int i = 0; i < enemyunits->fighters.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->fighters[i]->getcomponent<transposecomponent>();
		std::shared_ptr<healthcomponent> enemyhealth = enemyunits->fighters[i]->getcomponent<healthcomponent>();
		std::shared_ptr<AIcomp> enemyai = enemyunits->fighters[i]->getcomponent<AIcomp>();

		distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
		distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8), 0.2, 1);

		healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
		healthscore = setclamp(linearandquadratic(0.0f, healthscore, 0.5), 0.4, 1);

		glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
		float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
		float degreecheck = glm::degrees(shotdegrees);

		anglescore = invert(degreecheck / 180.0f, 1);
		anglescore = setclamp(logisticscore(2, anglescore, 0.6, -10), 0.2, 1);

		float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);

		if (enemyai->myaction != NULL)
		{
			if (enemyai->myaction->returntype() == "fleeaction" || enemyai->myaction->returntype() == "manaction")
			{
				newscore = newscore * 0.6f;
			}
		}

		if (newscore > bestscore)
		{
			besttarget = enemyunits->fighters[i];
			bestscore = newscore;
		}

	}
	if (enemyunits->player != NULL)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->player->getcomponent<transposecomponent>();
		std::shared_ptr<healthcomponent> enemyhealth = enemyunits->player->getcomponent<healthcomponent>();

		distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 1000) / 1000, 1);
		distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8), 0.2, 1);

		healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
		healthscore = setclamp(linearandquadratic(0.0f, healthscore, 0.5), 0.5, 1);

		glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
		float shotdegrees = acos(glm::dot(mydir->newdirect, norm));
		float degreecheck = glm::degrees(shotdegrees);

		anglescore = invert(degreecheck / 180.0f, 1);
		anglescore = setclamp(logisticscore(2, anglescore, 0.6, -10), 0.3, 1);

		float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);

		if (newscore > bestscore)
		{
			besttarget = enemyunits->player;
			bestscore = newscore;

			std::shared_ptr<directioncomponent> playerdir = besttarget->getcomponent<directioncomponent>();
			std::shared_ptr<healthcomponent> aihealth = me->getcomponent<healthcomponent>();

			glm::vec3 pnorm = glm::normalize((mytrans->position - enemytrans->position));
			float pshotdegrees = acos(glm::dot(playerdir->newdirect, pnorm));
			float pdegreecheck = glm::degrees(pshotdegrees);
			float panglescore = invert(pdegreecheck / 180.0f, 1);
			panglescore = setclamp(logisticscore(2, panglescore, 0.6, -10), 0.3, 1);

			float phealthscore = invert(aihealth->health / aihealth->maxhp, 1);
			phealthscore = setclamp(linearandquadratic(0.0f, phealthscore, 0.5), 0.5, 1);
			float playerscore = scoringfixer(scoringfixer(phealthscore, distancescore), panglescore);

			if (playerscore > bestscore)
			{
				bestscore = newscore;
				manaction newaction;
				newaction.setflee(besttarget);
				newaction.settime(touse.fulltime);
				newaction.addme(me);
				newaction.setman(touse);
				newaction.start();
				myaicomp->myaction = std::make_unique<manaction>(newaction);
				return;
			}

		}
	}

	if (myaicomp->score < bestscore)
	{
		myaicomp->score = bestscore;
		attackaction newaction;
		newaction.setteam(allyteam);
		newaction.gettarget(besttarget);
		newaction.addme(me);
		newaction.settime(10.0f);
		newaction.start();
		myaicomp->myaction = std::make_unique<attackaction>(newaction);

		std::shared_ptr<AIcomp> enemyaicomp = besttarget->getcomponent<AIcomp>();
		if (enemyaicomp != NULL)
		{
			if (enemyaicomp->score < bestscore)
			{
				enemyaicomp->score = bestscore;
				int i = rand() % 2;
				if (i == 0)
				{
					fleeaction newaction;
					newaction.setflee(me);
					newaction.settime(10.0f);
					newaction.addme(besttarget);
					newaction.start();
					enemyaicomp->myaction = std::make_unique<fleeaction>(newaction);
				}
				else
				{
					manaction newaction;
					newaction.setflee(me);
					newaction.settime(touse.fulltime);
					myaicomp->myaction->settime(touse.fulltime);
					newaction.addme(besttarget);
					newaction.setman(touse);
					newaction.start();
					enemyaicomp->myaction = std::make_unique<manaction>(newaction);
				}
			}
		}
	}
}


void turretidel(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, float maxdistance)
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();

	for (int i = 0; i < enemyunits->allunits.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = enemyunits->allunits[i]->getcomponent<transposecomponent>();

		if (glm::length(enemytrans->position - mytrans->position) < maxdistance)
		{
			return;
		}
	}

	idelaction newaction;
	newaction.settime(2.0f);
	newaction.start();
	myaicomp->myaction = std::make_unique<idelaction>(newaction);
	myaicomp->score = 1.0f;

}

void turrettarget(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits)
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<AIcomp> myaicomp = me->getcomponent<AIcomp>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();
	std::shared_ptr<turretcomp> myturrcomp = me->getcomponent<turretcomp>();

	std::shared_ptr<ACC::entity> besttarget;
	float bestscore = 0.0;

	float healthscore;
	float distancescore;
	float anglescore;
	float flyertypescore;


	for (int i = 0; i < enemyunits->allunits.size(); i++)
	{
		if (flyerunittypes(enemyunits->allunits[i]))
		{
			std::shared_ptr<transposecomponent> enemytrans = enemyunits->allunits[i]->getcomponent<transposecomponent>();
			std::shared_ptr<healthcomponent> enemyhealth = enemyunits->allunits[i]->getcomponent<healthcomponent>();

			if (mytrans->position.y >= mytrans->position.y)
			{
				distancescore = invert(std::fmin(glm::length(mytrans->position - enemytrans->position), 600.0f) / 600.0f, 1);
				distancescore = setclamp(linearandquadratic(0.0f, distancescore, 0.8), 0.2, 1);
			}
			else
			{
				distancescore = 0.0f;
			}

			healthscore = invert(enemyhealth->health / enemyhealth->maxhp, 1);
			healthscore = setclamp(linearandquadratic(0.0f, healthscore, 1.3), 0.1, 1);

			glm::vec3 norm = glm::normalize((enemytrans->position - mytrans->position));
			float shotdegrees = acos(glm::dot(myturrcomp->basedir, norm));
			float degreecheck = glm::degrees(shotdegrees);

			anglescore = invert(degreecheck / 40.0f, 1);
			anglescore = setclamp(logisticscore(2, anglescore, 0.7, -10), 0.1, 1);

			float newscore = scoringfixer(scoringfixer(healthscore, distancescore), anglescore);

			if (newscore > bestscore)
			{
				besttarget = enemyunits->allunits[i];
				bestscore = newscore;
			}

		}
	}

	if (myaicomp->score < bestscore)
	{
		turretfireaction newaction;
		newaction.settarget(besttarget);
		newaction.settime(10.0f);
		newaction.addme(me);
		newaction.start();
		myaicomp->myaction = std::make_unique<turretfireaction>(newaction);
	}


}

bool flyerunittypes(std::shared_ptr<ACC::entity> unit)
{
	if(unit->returntype() == enttypes::Player || unit->returntype() == enttypes::Fighter || unit->returntype() == enttypes::Bomber)
	{
		return true;
	}

	return false;
}
