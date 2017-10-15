#include "system_manager.h"
#include "scene.h"

ACC::System_Manager::System_Manager(std::shared_ptr<scene> s, std::shared_ptr<world> w)
{
	myscene = s;
	myworld = w;
}

void ACC::System_Manager::updatesystements() //add new entities to the systems if any have been created last frame 
{
	std::vector<std::shared_ptr<ACC::entity>> toadd = myworld->returnmanager()->returntoadd();

	if (toadd.size() == 0)
	{
		return;
	}

	for (auto syetm : A_Systems)
	{
		syetm->soiwant(toadd);
	}
	if (rendersystem != NULL)
	{
		rendersystem->soiwant(toadd);
	}

	myworld->returnmanager()->cleartoadd();
}

void ACC::System_Manager::deletesystements() //delete any entities from systems that we no longer need
{
	std::vector<std::shared_ptr<ACC::entity>> todelete = myworld->returnmanager()->returntodelete();

	if (todelete.size() == 0)
	{
		return;
	}

	for (auto syetm : A_Systems)
	{
		syetm->doihave(todelete);
	}
	if (rendersystem != NULL)
	{
		rendersystem->doihave(todelete);
	}

	myworld->returnmanager()->cleartodelete();


}

void ACC::System_Manager::addsystem(std::shared_ptr<System> system)
{
	A_Systems.push_back(system); // add a new system

}

void ACC::System_Manager::update(float dt, bool &go)
{
	for (int i = 0; i < A_Systems.size(); i++) // update all our systems
	{
		A_Systems[i]->update(dt, go);
	}
}

void ACC::System_Manager::render(float dt, bool &go)
{
	if (rendersystem != NULL)
	{
		rendersystem->update(dt, go); // render all the model entities
	}
}

bool ACC::System_Manager::cleanupsystems() // clean up all our systems and the render system
{
	for (int i = 0; i < A_Systems.size(); i++)
	{
		A_Systems.erase(A_Systems.begin() + i);
		i--;
	}

	return true;
}

void ACC::System_Manager::setrender(std::shared_ptr<System> rend) // set the render system
{
	rendersystem = rend;
}

