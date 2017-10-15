#pragma once

#include "System.h"
#include <vector>

class SoundSystem : public ACC::System
{
private:

	std::vector<std::shared_ptr<ACC::entity>> soundobjects;
	float volume;

public:

	SoundSystem(std::shared_ptr<world> w, float vol);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);


};