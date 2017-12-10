#include "TurretAttack.h"
#include "entity.h"
#include "world.h"
#include "Teamunits.h"
#include "OctTree.h"

void TurretAttack::setmyman(maneuver mym)
{
	myman = mym;

	myai = me->getcomponent<AIcomp>();
	mytrans = me->getcomponent<transposecomponent>();
	myphys = me->getcomponent<physics>();
	mydir = me->getcomponent<directioncomponent>();
	mycol = me->getcomponent<collisioncomp>();
	enemytrans = mytarget->getcomponent<transposecomponent>();
	enemycol = mytarget->getcomponent<collisioncomp>();
	myanimation = mytarget->getcomponent<animationholdercomp>();
	capitalcol = myanimation->myowner->getcomponent<collisioncomp>();
}

void TurretAttack::setcaptarget(std::shared_ptr<ACC::entity> captar)
{
	mytarget = captar;

}

void TurretAttack::start()
{
	man = false;
	roll = 0;
	mannum = 0;

}

void TurretAttack::run(float dt, std::shared_ptr<world> myworld)
{

	float lookahead = glm::max((glm::length(myphys->velocity) / myphys->maxspeed) * 100, mycol->mysphere->rad);

	glm::vec3 seekforce = seekforcecal(dt);

	float targetdistance = glm::length(mytrans->position - enemytrans->position);

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

	myphys->force = seekforce + avoidforce;

	float t;

	if (enemycol->mysphere->raycollision(newray, t) && targetdistance < 600)
	{
		std::shared_ptr<fighterguns> myguns = me->getcomponent<fighterguns>();
		if (myguns->currettime >= myguns->firetime)
		{
			myguns->fire = true;
		}
	}

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

void TurretAttack::end()
{


}

glm::vec3 TurretAttack::seekforcecal(float dt)
{
	glm::vec3 seekforce;

	float targetdistance = glm::length(mytrans->position - enemytrans->position);

	if (man == false)
	{
		if (targetdistance < (mycol->mysphere->rad + enemycol->mysphere->rad + 2.0f) || targetdistance < (mycol->mysphere->rad + capitalcol->mysphere->rad + 2.0f))
		{
			man = true;
			for (int i = 0; i < myman.manpos.size(); i++)
			{
				float lenght = glm::length(myman.manpos[i]);
				myman.manpos[i] = mytrans->myquat * glm::normalize(myman.manpos[i]);
				myman.manpos[i] *= lenght;
			}
		}
		else
		{
			seekforce = myai->MovCon.seekobject(enemytrans->position) * 0.7f;
		}
	}
	else
	{
		seekforce = myai->MovCon.seekobject(myman.manpos[mannum] + mytrans->position) * 0.7f;

		if (roll < myman.roll[mannum])
		{
			roll++;
		}
		else if (roll > myman.roll[mannum])
		{
			roll--;
		}

		if (roll > 360)
		{
			roll = 0;
		}

		myman.time[mannum] -= dt;

		if (myman.time[mannum] <= 0)
		{
			mannum++;
		}
		if (mannum == myman.manpos.size())
		{
			time = 0;
		}
	}

	return seekforce;
}
