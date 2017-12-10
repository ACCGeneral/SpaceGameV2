#include "fleeaction.h"
#include "entity.h"
#include "world.h"
#include "OctTree.h"

void fleeaction::setflee(std::shared_ptr<ACC::entity> flee)
{
	fleefrom = flee;

}

void fleeaction::start()
{

	myai = me->getcomponent<AIcomp>();
	mytrans = me->getcomponent<transposecomponent>();
	fleetrans = fleefrom->getcomponent<transposecomponent>();
	mydir = me->getcomponent<directioncomponent>();
	myphyscomp = me->getcomponent<physics>();
	mycol = me->getcomponent<collisioncomp>();

}

void fleeaction::run(float dt, std::shared_ptr<world> myworld)
{
	if (fleefrom->returndeleteme())
	{
		fleefrom = NULL;
		time = 0;
		return;
	}

	

	float lookahead = glm::max((glm::length(myphyscomp->velocity) / myphyscomp->maxspeed) * 80, mycol->mysphere->rad);

	glm::vec3 fleeforce = myai->MovCon.fleefromobject(fleetrans->position) * 0.7f;

	Ray newray;
	newray.pos = mytrans->position;
	newray.dir = mydir->newdirect;
	std::vector<int> idstomiss{me->returnID()};
	myworld->returnoct()->getclosesttoray(newray, idstomiss, lookahead);

	glm::vec3 avoidforce = glm::vec3(0,0,0);

	raycollinfo raycol = myworld->returnoct()->returnraycol();

	if (raycol.hit == true)
	{
		avoidforce = myai->MovCon.avoidobject(raycol.hitpoint, raycol.center);
	}

	myphyscomp->force = fleeforce + avoidforce;

	glm::vec3 feelvec = myphyscomp->velocity + mytrans->position;


	if (feelvec.x == mytrans->position.x && feelvec.z == mytrans->position.z)
	{
		feelvec.x += 1;
	}


	glm::mat4 lookatmatrix = glm::transpose(glm::lookAt(feelvec, mytrans->position, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::quat newrotation = glm::toQuat(lookatmatrix);
	float angleofRot = glm::clamp(2.0f * dt, 0.0f, 1.0f);
	mytrans->myquat = glm::slerp(mytrans->myquat, newrotation, angleofRot);
	mydir->newdirect = mytrans->myquat * mydir->direction;

	time -= dt;

}

void fleeaction::end()
{


}
