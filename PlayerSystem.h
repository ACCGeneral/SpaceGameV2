#pragma once

#include "System.h"
#include <vector>
#include <AL/al.h>   
#include <AL/alc.h>  

class PlayerSystem : public ACC::System
{

private:

	std::shared_ptr<ACC::entity> player;


public:


	PlayerSystem(std::shared_ptr<world> w);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);


};