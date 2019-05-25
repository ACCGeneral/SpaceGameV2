#include "CollisionSystem.h"
#include "OctTree.h"
#include "Octinfo.h"

CollisionSystem::CollisionSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "CollisionSystem";
	root = myworld->returnoct();
}

void CollisionSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<unsigned long int> lookfor;
	lookfor.push_back(transposecomponent::TypeID);
	lookfor.push_back(collisioncomp::TypeID);

	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(lookfor))
		{
			std::shared_ptr<collisioncomp> entcoldata = ent[i]->getcomponent<collisioncomp>();
			collisionEnts.push_back(ent[i]);
			Octinfo newinfo;
			newinfo.objectaabb = entcoldata->myAABB;
			newinfo.objectobb = entcoldata->myobb;
			newinfo.objectsphere = entcoldata->mysphere;
			newinfo.unitID = ent[i]->returnID();
			newinfo.cantmove = entcoldata->imstatic;
			newinfo.physicsobj = entcoldata->physicseffected;
			newinfo.cantcollide = entcoldata->dontcolwith;
			newinfo.bullettype = entcoldata->bullet;
			root->addobject(newinfo);
		}
	}
}

void CollisionSystem::update(float & dt, bool & go)
{

	for (int i = 0; i < collisionEnts.size(); i++)
	{
		std::shared_ptr<collisioncomp> entcoldata = collisionEnts[i]->getcomponent<collisioncomp>();
		std::shared_ptr<transposecomponent> enttrans = collisionEnts[i]->getcomponent<transposecomponent>();
		std::shared_ptr<physics> entonevel = collisionEnts[i]->getcomponent<physics>();
		glm::vec3 posent1 = enttrans->position;

		entcoldata->myAABB->update(enttrans->myquat, posent1);
		entcoldata->mysphere->update(posent1);
		for (int i = 0; i < entcoldata->myobb.size(); i++)
		{
			entcoldata->myobb[i]->update(posent1, enttrans->myquat);
		}

		if (enttrans->position.x > 4000 || enttrans->position.y > 4000 || enttrans->position.z > 4000 || enttrans->position.x < -4000 || enttrans->position.y < -4000 || enttrans->position.z < -4000)
		{
			myworld->returnmanager()->addtodeletelist(collisionEnts[i]);
		}
	}
	root->update();

}


void CollisionSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < collisionEnts.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (collisionEnts[i] == ents[j])
			{
				bool removefrom = false;
				root->removethis(ents[j]->returnID(), removefrom);
				collisionEnts.erase(collisionEnts.begin() + i);
				ents.erase(ents.begin() + j);
				j--;
				i--;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}

}
