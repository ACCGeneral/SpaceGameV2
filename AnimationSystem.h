#pragma once

#include "System.h"
#include <vector>

class AnimationSystem : public ACC::System
{
private:

	std::vector<std::shared_ptr<ACC::entity>> animatedobjects;

public:

	AnimationSystem(std::shared_ptr<world> w);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);


};