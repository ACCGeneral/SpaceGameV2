#include "attackaction.h"
#include "entity.h"
#include "world.h"
#include "Teamunits.h"
#include "OctTree.h"

void attackaction::setteam(std::shared_ptr<teamunits> team)
{
	myteam = team;
}

void attackaction::gettarget(std::shared_ptr<ACC::entity> tar)
{
	mytarget = tar;

}

void attackaction::start()
{
	std::shared_ptr<physics> myphyscomp = me->getcomponent<physics>();

	orgmaxvel = myphyscomp->maxspeed;
}

void attackaction::run(float dt, std::shared_ptr<world> myworld)
{
	std::shared_ptr<AIcomp> myai = me->getcomponent<AIcomp>();

	if (mytarget->returndeleteme())
	{
		mytarget = NULL;
		time = 0;
		return;
	}

	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<transposecomponent> attacktrans = mytarget->getcomponent<transposecomponent>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();
	std::shared_ptr<collisioncomp> mycol = me->getcomponent<collisioncomp>();
	std::shared_ptr<physics> myphyscomp = me->getcomponent<physics>();

	std::shared_ptr<collisioncomp> enemycol = mytarget->getcomponent<collisioncomp>();

	glm::vec3 seekforce;

	glm::vec3 seperationforce = myai->MovCon.seperation(myteam) * 0.4f;

	float lookahead = glm::max((glm::length(myphyscomp->velocity) / myphyscomp->maxspeed) * 100, mycol->mysphere->rad);

	float targetdistance = glm::length(mytrans->position - attacktrans->position);

	if (targetdistance <= 200)
	{
		std::shared_ptr<physics> enemyphys = mytarget->getcomponent<physics>();
		myphyscomp->maxspeed = enemyphys->maxspeed - 5;
	}
	else
	{
		myphyscomp->maxspeed = orgmaxvel;
	}

	seekforce = myai->MovCon.seekobject(enemycol->myAABB->mypos) * 0.8f;
	
	Ray newray;
	newray.pos = mytrans->position;
	newray.dir = mydir->newdirect;
	std::vector<int> idstomiss{ me->returnID() };
	myworld->returnoct()->getclosesttoray(newray, idstomiss, lookahead);

	glm::vec3 avoidforce = glm::vec3(0, 0, 0);

	raycollinfo raycol = myworld->returnoct()->returnraycol();

	if (raycol.hit == true)
	{
		if (raycol.closestID != mytarget->returnID() || raycol.t < mycol->mysphere->rad + 80)
		{
			avoidforce = myai->MovCon.avoidobject(raycol.hitpoint, raycol.center);
		}
	}

	glm::vec3 norm = glm::normalize((attacktrans->position - mytrans->position));

	float shotdegrees = acos(glm::dot(mydir->newdirect, norm));

	if (glm::degrees(shotdegrees) < enemycol->mysphere->rad && targetdistance < 500)
	{
		std::shared_ptr<fighterguns> myguns = me->getcomponent<fighterguns>();
		if (myguns->currettime >= myguns->firetime)
		{
			myguns->fire = true;
		}
	}

	myphyscomp->force = seekforce + avoidforce + seperationforce;
	glm::vec3 attackvec;

	if (glm::length(myphyscomp->velocity) < 5)
	{
		attackvec = attacktrans->position;
	}
	else
	{
		attackvec = myphyscomp->velocity + mytrans->position;
	}

	if (attackvec.x == mytrans->position.x && attackvec.z == mytrans->position.z)
	{
		attackvec.x += 1;
	}


	glm::mat4 lookatmatrix = glm::transpose(glm::lookAt(attackvec, mytrans->position, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::quat newrotation = glm::toQuat(lookatmatrix);
	float angleofRot = glm::clamp(2.0f * dt, 0.0f, 1.0f);
	mytrans->myquat = glm::slerp(mytrans->myquat, newrotation, angleofRot);
	mydir->newdirect = mytrans->myquat * mydir->direction;

	time -= dt;
}

void attackaction::end()
{
	std::shared_ptr<physics> myphyscomp = me->getcomponent<physics>();
	myphyscomp->maxspeed = orgmaxvel;
}
