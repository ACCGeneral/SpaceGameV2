#include "turretfireaction.h"
#include "entity.h"
#include "world.h"
#include "OctTree.h"

void turretfireaction::firegunstest(float dt)
{
	
	glm::vec3 realgunpos = glm::vec3(mygunstrans->modelMatrix[3][0], mygunstrans->modelMatrix[3][1], mygunstrans->modelMatrix[3][2]);

	glm::vec3 normdistance = glm::normalize(targettrans->position - realgunpos);
	float insights = acos(glm::dot(normdistance, myturretcomp->basedir));
	if (glm::abs(glm::degrees(insights)) <= 40)
	{
		float fireangle = acos(glm::dot(normdistance, gundir->newdirect));
		if (glm::abs(glm::degrees(fireangle)) < enemycol->mysphere->rad)
		{
			if (myguns->currettime >= myguns->firetime)
			{
				myguns->fire = true;
				timenotshot = 0;
			}
		}

		glm::vec3 NoenemyY = glm::vec3(targettrans->position.x, targettrans->position.y, targettrans->position.z);
		float lenghttotarget = glm::length(NoenemyY - realgunpos);
		glm::vec3 dirtimeslenghtto = baseturretdir->direction * lenghttotarget;
		dirtimeslenghtto += glm::vec3(realgunpos.x, targettrans->position.y, realgunpos.z);

		glm::mat4 tlookatmatrix = glm::transpose(glm::lookAt(targettrans->position, realgunpos, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::quat tnewrotation = glm::toQuat(tlookatmatrix);

		glm::mat4 lookatmatrix = glm::transpose(glm::lookAt(dirtimeslenghtto, realgunpos, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::quat newrotation = glm::toQuat(lookatmatrix);
		float angleofRot = glm::clamp(4.0f * dt, 0.0f, 1.0f);
		gunsanim->myanimation = glm::slerp(gunsanim->myanimation, newrotation, angleofRot);
	}
	else
	{
		time = 0;
	}

	gundir->newdirect = mygunstrans->myquat * gundir->direction;

}

void turretfireaction::settarget(std::shared_ptr<ACC::entity> tar)
{
	target = tar;

}

void turretfireaction::start()
{
	std::shared_ptr<thingsiown> iown = me->getcomponent<thingsiown>();

	for (int i = 0; i < iown->stuffiown.size(); i++)
	{
		if (iown->stuffiown[i]->returntype() == enttypes::Turretguns)
		{
			turretguns = iown->stuffiown[i];
			break;
		}
	}

	mytrans = me->getcomponent<transposecomponent>();
	myanim = me->getcomponent<animationholdercomp>();
	myturretcomp = me->getcomponent<turretcomp>();
	mydir = me->getcomponent<directioncomponent>();
	baseturretdir = me->getcomponent<directioncomponent>();

	mygunstrans = turretguns->getcomponent<transposecomponent>();
	targettrans = target->getcomponent<transposecomponent>();
	gundir = turretguns->getcomponent<directioncomponent>();
	gunsanim = turretguns->getcomponent<animationholdercomp>();
	myguns = turretguns->getcomponent<fighterguns>();

	targettrans = target->getcomponent<transposecomponent>();
	enemycol = target->getcomponent<collisioncomp>();
}

void turretfireaction::run(float dt, std::shared_ptr<world> myworld)
{
	timenotshot += dt;

	if (target->returndeleteme() || timenotshot >= 3.0)
	{
		target = NULL;
		time = 0;
		return;
	}

	glm::vec3 normYEnemyVec = glm::vec3(targettrans->position.x, mytrans->position.y, targettrans->position.z);

	float dista = glm::length(targettrans->position - mytrans->position);

	glm::vec3 directionto = glm::normalize(normYEnemyVec - mytrans->position);

	float anglebetween = acos(glm::dot(directionto, myturretcomp->basedir));

	if (glm::degrees(glm::abs(anglebetween)) <= 60 && dista < 600)
	{
		glm::vec3 lookat = normYEnemyVec;
		glm::mat4 lookatmatrix = glm::transpose(glm::lookAt(lookat, mytrans->position, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::quat newrotation = glm::toQuat(lookatmatrix);
		float angleofRot = glm::clamp(4.0f * dt, 0.0f, 1.0f);
		myanim->myanimation = glm::slerp(myanim->myanimation, newrotation, angleofRot);
		mydir->newdirect = mytrans->myquat * mydir->direction;
		firegunstest(dt);
		time -= dt;
	}
	else
	{
		time = 0;
	}

}


void turretfireaction::end()
{

}