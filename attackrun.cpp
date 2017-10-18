#include "attackrun.h"
#include "entity.h"
#include "world.h"
#include "OctTree.h"

void attackrunaction::setmyman(maneuver mym)
{
	myman = mym;

}

void attackrunaction::setattackman(maneuver mym)
{
	attackman = mym;
}

void attackrunaction::gettarget(std::shared_ptr<ACC::entity> tar)
{
	mytarget = tar;
}

void attackrunaction::start()
{
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<transposecomponent> enemycapsing = mytarget->getcomponent<transposecomponent>();
	glm::quat tocap = glm::toQuat(glm::lookAt(mytrans->position, enemycapsing->position,glm::vec3(0,1,0)));

	for (int i = 0; i < attackman.manpos.size(); i++)
	{
		float lenght = glm::length(attackman.manpos[i]);
		attackman.manpos[i] = tocap * glm::normalize(attackman.manpos[i]);
		attackman.manpos[i] *= lenght;
	}

	activeman = attackman;

}

void attackrunaction::run(float dt, std::shared_ptr<world> myworld)
{
	std::shared_ptr<AIcomp> myai = me->getcomponent<AIcomp>();
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<physics> myphys = me->getcomponent<physics>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();
	std::shared_ptr<collisioncomp> mycol = me->getcomponent<collisioncomp>();

	std::shared_ptr<transposecomponent> enemytrans = mytarget->getcomponent<transposecomponent>();
	std::shared_ptr<collisioncomp> enemycol = mytarget->getcomponent<collisioncomp>();

	float lookahead = glm::max((glm::length(myphys->velocity) / myphys->maxvelocity) * 100, mycol->mysphere->rad);

	glm::vec3 seekforce;
	float targetdistance = glm::length(mytrans->position - enemytrans->position);

	if (targetdistance < (mycol->mysphere->rad + enemycol->mysphere->rad + 2.0f))
	{
		for (int i = 0; i < myman.manpos.size(); i++)
		{
			float lenght = glm::length(myman.manpos[i]);
			myman.manpos[i] = mytrans->myquat * glm::normalize(myman.manpos[i]);
			myman.manpos[i] *= lenght;
		}

		activeman = myman;
		mannum = 0;
	}


	seekforce = myai->MovCon.seekobject(mytrans->position + activeman.manpos[mannum]) * 0.7f;

	if (roll < activeman.roll[mannum])
	{
		roll++;
	}
	else if (roll > activeman.roll[mannum])
	{
		roll--;
	}

	if (roll > 360)
	{
		roll = 0;
	}

	activeman.time[mannum] -= dt;

	if (activeman.time[mannum] <= 0)
	{
		mannum++;
	}
	if (mannum == activeman.manpos.size())
	{
		if (activeman.fulltime == attackman.fulltime)
		{
			activeman = myman;
			mannum = 0;
		}
		else
		{
			time = 0;
		}
	}

	Ray newray;
	newray.pos = mytrans->position;
	newray.dir = mydir->newdirect;
	std::vector<int> idstomiss{ me->returnID() };
	myworld->returnoct()->getclosesttoray(newray, idstomiss, lookahead);

	glm::vec3 avoidforce = glm::vec3(0, 0, 0);

	raycollinfo raycol = myworld->returnoct()->returnraycol();

	if (raycol.hit == true)
	{
		avoidforce = myai->MovCon.avoidobject(raycol.hitpoint, raycol.center);
	}

	float t;

	if (enemycol->myAABB->raycollision(newray,t) && targetdistance < 500)
	{
		std::shared_ptr<fighterguns> myguns = me->getcomponent<fighterguns>();
		if (myguns->currettime >= myguns->firetime)
		{
			myguns->fire = true;
		}
	}

	myphys->force = seekforce + avoidforce;

	glm::vec3 feelvec = myphys->velocity + mytrans->position;

	if (feelvec.x == mytrans->position.x && feelvec.z == mytrans->position.z)
	{
		feelvec.x += 1;
	}

	glm::mat4 lookatmatrix = glm::transpose(glm::lookAt(feelvec, mytrans->position, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::quat newrotation = glm::toQuat(lookatmatrix);
	glm::vec3 zaxis = newrotation * glm::vec3(0, 0, 1);
	glm::quat zroll = glm::angleAxis(glm::radians((float)roll), zaxis);
	newrotation = zroll * newrotation;
	float angleofRot = glm::clamp(2.0f * dt, 0.0f, 1.0f);
	mytrans->myquat = glm::slerp(mytrans->myquat, newrotation, angleofRot);
	mydir->newdirect = mytrans->myquat * mydir->direction;

	time -= dt;
}

void attackrunaction::end()
{



}
