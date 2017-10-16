#include "scene.h"
#include "RenderSystem.h"
#include "System.h"

void scene::updateEnts()
{
	std::vector<std::shared_ptr<ACC::entity>> toadd = myworld->returnmanager()->returntoadd();

	if (toadd.size() == 0)
	{
		return;
	}

	for (auto syetm : myupdatesystems)
	{
		syetm->soiwant(toadd);
	}
	if (myRenderSystem != NULL)
	{
		myRenderSystem->soiwant(toadd);
	}

	myworld->returnmanager()->cleartoadd();
}

void scene::destroyEnts()
{
	std::vector<std::shared_ptr<ACC::entity>> todelete = myworld->returnmanager()->returntodelete();

	if (todelete.size() == 0)
	{
		return;
	}

	for (auto syetm : myupdatesystems)
	{
		syetm->doihave(todelete);
	}
	if (myRenderSystem != NULL)
	{
		myRenderSystem->doihave(todelete);
	}

	myworld->returnmanager()->cleartodelete();
}

scene::scene(std::shared_ptr<SceneManager> SM) // get relevent data 
{
	myscenemanager = SM;

}
