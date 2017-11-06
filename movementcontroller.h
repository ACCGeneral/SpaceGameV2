#pragma once

#include <memory>
#include "glm.hpp"

class teamunits;

namespace ACC
{
	class entity;
};

class movementcontroller
{

private:

	std::shared_ptr<ACC::entity> myentity;
	float seperationsphere;
	float maxforce;
	glm::vec3 newforce;

public:

	movementcontroller()
	{
		maxforce = 0;
		newforce = glm::vec3(0, 0, 0);
	}

	void setme(std::shared_ptr<ACC::entity> me);
	void setsepsphere(float sep);

	void setmaxforce(float max_force)
	{
		maxforce = max_force;
	}
	void reset()
	{
		newforce = glm::vec3(0, 0, 0);
	}
	glm::vec3 returnForce()
	{
		return newforce;
	}

	glm::vec3 seekobject(glm::vec3 toseek);

	glm::vec3 fleefromobject(glm::vec3 tofeel);

	glm::vec3 arrivetobject(glm::vec3 toseek, float slowrad);

	glm::vec3 avoidobject(glm::vec3 hitpos, glm::vec3 centerpoint);

	glm::vec3 speedmatc(glm::vec3 othervel, float maxforce);

	glm::vec3 seperation(std::shared_ptr<teamunits> myteam);

};


