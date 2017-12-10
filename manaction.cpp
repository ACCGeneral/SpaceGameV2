#include "manaction.h"
#include "entity.h"
#include "world.h"
#include "OctTree.h"

void manaction::setflee(std::shared_ptr<ACC::entity> flee)
{
	fleefrom = flee;
}

void manaction::setman(maneuver mym)
{
	myman = mym;
}

void manaction::start()
{
	roll = 0;
	mannum = 0;

	mytrans = me->getcomponent<transposecomponent>();
	myphys = me->getcomponent<physics>();
	mydir = me->getcomponent<directioncomponent>();
	myai = me->getcomponent<AIcomp>();
	mycol = me->getcomponent<collisioncomp>();

	for (int i = 0; i < myman.manpos.size(); i++)
	{
		float lenght = glm::length(myman.manpos[i]);
		myman.manpos[i] = mytrans->myquat * glm::normalize(myman.manpos[i]);
		myman.manpos[i] *= lenght;
	}

}

void manaction::run(float dt, std::shared_ptr<world> myworld)
{

	if (fleefrom->returndeleteme())
	{
		fleefrom = NULL;
		time = 0;
		return;
	}

	float lookahead = glm::max((glm::length(myphys->velocity) / myphys->maxspeed) * 80, mycol->mysphere->rad);

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
	glm::vec3 seekforce = myai->MovCon.seekobject(mytrans->position + myman.manpos[mannum]) * 0.7f;

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
	time -= dt;

	myphys->force = avoidforce + seekforce;


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
}

void manaction::end()
{


}
