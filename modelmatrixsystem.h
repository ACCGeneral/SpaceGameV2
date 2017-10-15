#pragma once

#include "System.h"
#include <vector>


class modelmatrixsystem : public ACC::System //updates model matrix for each entity
{

private:

	std::vector<std::shared_ptr<ACC::entity>> transformEnts;


public:

	modelmatrixsystem(std::shared_ptr<world> w);

	void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent);

	void update(float &dt, bool &go);

	std::string returnname()
	{
		return my_System_Name;
	}

	void doihave(std::vector<std::shared_ptr<ACC::entity>> ent);

};