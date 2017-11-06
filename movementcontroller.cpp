#include "movementcontroller.h"
#include "entity.h"
#include "Teamunits.h"


void movementcontroller::setme(std::shared_ptr<ACC::entity> me)
{
	myentity = me;
}

void movementcontroller::setsepsphere(float sep)
{
	seperationsphere = sep;
}

glm::vec3 movementcontroller::seekobject(glm::vec3 toseek)
{
	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	glm::vec3 seekforce = glm::normalize(toseek - mytrans->position) * myphys->maxspeed;
	return seekforce - myphys->velocity;
}

glm::vec3 movementcontroller::fleefromobject(glm::vec3 tofeel)
{
	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	glm::vec3 fleeforce = glm::normalize(mytrans->position - tofeel) * myphys->maxspeed;
	return fleeforce - myphys->velocity;
}

glm::vec3 movementcontroller::arrivetobject(glm::vec3 toseek, float slowrad)
{
	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	glm::vec3 wantedvel = toseek - mytrans->position;
	float distancelengh = glm::length(wantedvel);
	glm::vec3 seekforce;

	if (distancelengh < slowrad)
	{
		seekforce = glm::normalize(wantedvel) * myphys->maxspeed * (distancelengh / slowrad);
	}
	else
	{
		seekforce = glm::normalize(wantedvel) * myphys->maxspeed;
	}

	return seekforce - myphys->velocity;
}

glm::vec3 movementcontroller::avoidobject(glm::vec3 hitpos, glm::vec3 centerpoint)
{
	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	hitpos += glm::vec3(0.01, 0.1, 0.011);
	glm::vec3 avoidforce = glm::normalize(hitpos - centerpoint) * myphys->maxspeed;
	return avoidforce - myphys->velocity;
}

glm::vec3 movementcontroller::speedmatc(glm::vec3 othervel, float maxforce)
{
	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	float totarget = 0.1;

	glm::vec3 matchforce = othervel - myphys->velocity;
	matchforce /= totarget;

	if (glm::length(matchforce) > maxforce)
	{
		matchforce = glm::normalize(matchforce) * maxforce;
	}

	return matchforce;
}

glm::vec3 movementcontroller::seperation(std::shared_ptr<teamunits> myteam)
{
	float neighbors = 0;

	std::shared_ptr<transposecomponent> mytrans = myentity->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = myentity->getcomponent<physics>();

	glm::vec3 seperationforce = glm::vec3(0.0f);

	for (int i = 0; i != myteam->allunits.size(); i++)
	{
		std::shared_ptr<transposecomponent> enemytrans = myteam->allunits[i]->getcomponent<transposecomponent>();

		if (myteam->allunits[i] != myentity)
		{
			glm::vec3 temp = mytrans->position - enemytrans->position;
			float dis = glm::length(temp);

			if (glm::abs(dis) <= seperationsphere)
			{
				seperationforce += glm::normalize(temp) / dis;
				neighbors++;
			}
		}

	}

	if (neighbors == 0)
	{
		return seperationforce = glm::vec3(0.0f);
	}

	seperationforce = glm::normalize(seperationforce) * myphys->maxspeed;
	return seperationforce - myphys->velocity;
}
