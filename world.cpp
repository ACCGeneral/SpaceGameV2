#include "world.h"
#include "entitymanager.h"
#include "OctTree.h"
#include "maincamera.h"
 
world::world(std::shared_ptr<entitymanager> getent) //add manager to world 
{
	myentmanager = getent;
}

void world::init()
{

}

void world::exit() //entity manager cleanup
{
	myentmanager->delete_entity();

}

void world::setoct(OctTree *myQ) //add quad tree 
{
	myocttree = myQ;

}

std::shared_ptr<entitymanager> world::returnmanager()
{
	return myentmanager;
}


