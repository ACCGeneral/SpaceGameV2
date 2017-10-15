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



}

void fleeaction::run(float dt, std::shared_ptr<world> myworld)
{
	if (fleefrom->returndeleteme())
	{
		fleefrom = NULL;
		time = 0;
		return;
	}

	std::shared_ptr<AIcomp> myai = me->getcomponent<AIcomp>();
	std::shared_ptr<transposecomponent> mytrans = me->getcomponent<transposecomponent>();
	std::shared_ptr<transposecomponent> fleetrans = fleefrom->getcomponent<transposecomponent>();
	std::shared_ptr<directioncomponent> mydir = me->getcomponent<directioncomponent>();
	std::shared_ptr<physics> myphyscomp = me->getcomponent<physics>();

	float lookahead = glm::max((glm::length(myphyscomp->velocity) / myphyscomp->maxvelocity) * 80, 12.0f);

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
