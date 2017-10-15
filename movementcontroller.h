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

public:

	movementcontroller()
	{

	}

	void setme(std::shared_ptr<ACC::entity> me);
	void setsepsphere(float sep);

	glm::vec3 seekobject(glm::vec3 toseek);

	glm::vec3 fleefromobject(glm::vec3 tofeel);

	glm::vec3 arrivetobject(glm::vec3 toseek, float slowrad);

	glm::vec3 avoidobject(glm::vec3 hitpos, glm::vec3 centerpoint);

	glm::vec3 speedmatc(glm::vec3 othervel, float maxforce);

	glm::vec3 seperation(std::shared_ptr<teamunits> myteam);

};
