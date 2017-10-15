#pragma once
#include <memory>

class entitymanager;
class OctTree;
class maincamera;

//our world class that contains its own entity manager aswell as other important classes (each scene has its own world)

class world
{
private:

	std::shared_ptr<entitymanager> myentmanager;
	OctTree *myocttree;
	std::shared_ptr<maincamera> maincam;

public:

	world()
	{
		
	}

	world(std::shared_ptr<entitymanager> getent);
	void init();
	void exit();
	void setoct(OctTree *myQ);
	
	std::shared_ptr<entitymanager> returnmanager();
	OctTree *returnoct()
	{
		return myocttree;
	}
	void setmaincam(std::shared_ptr<maincamera> cam)
	{
		maincam = cam;
	}
	std::shared_ptr<maincamera> getmaincam()
	{
		return maincam;
	}
	
};