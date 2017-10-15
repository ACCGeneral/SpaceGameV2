#include "CollisionSystem.h"
#include "Octinfo.h"
#include "OctTree.h"

CollisionSystem::CollisionSystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "CollisionSystem";
	physicssound = Resourecmanager::instance().getsound("Collision");

}

void CollisionSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	OctTree *root = myworld->returnoct();
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
			root->addobject(newinfo);
		}
	}
}

void CollisionSystem::update(float & dt, bool & go)
{
	OctTree *root = myworld->returnoct();
	bool removefrom = false;

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

		removefrom = false;
		if (enttrans->position.x > 4000 || enttrans->position.y > 4000 || enttrans->position.z > 4000 || enttrans->position.x < -4000 || enttrans->position.y < -4000 || enttrans->position.z < -4000)
		{
			myworld->returnmanager()->addtodeletelist(collisionEnts[i]);
			root->removethis(collisionEnts[i]->returnID(), removefrom);
		}

	}

	root->update();

	dealwithcols();

	root = NULL;

}

void CollisionSystem::dealwithcols()
{
	OctTree *root = myworld->returnoct();
	bool removefrom = false;

	std::vector<collisioninfo> mycols = root->returncolls();

	for (int i = 0; i < mycols.size(); i++)
	{
		removefrom = false;

		std::shared_ptr<ACC::entity> entone = myworld->returnmanager()->getentfromid(mycols[i].ID1);
		std::shared_ptr<ACC::entity> enttwo = myworld->returnmanager()->getentfromid(mycols[i].ID2);

		std::shared_ptr<collisioncomp> entonecol = entone->getcomponent<collisioncomp>();
		std::shared_ptr<collisioncomp> enttwocol = enttwo->getcomponent<collisioncomp>();

		std::shared_ptr<physics> entonevel = entone->getcomponent<physics>();
		std::shared_ptr<physics> enttwovel = enttwo->getcomponent<physics>();

		std::shared_ptr<transposecomponent> entonetrans = entone->getcomponent<transposecomponent>();
		std::shared_ptr<transposecomponent> enttwotrans = enttwo->getcomponent<transposecomponent>();

		if (entonecol->physicseffected && enttwocol->physicseffected)
		{
			glm::vec3 collisionpos = glm::vec3(0,0,0);

			if (!entonecol->imstatic && entonevel)
			{
				entonetrans->position += (mycols[i].projectionaxis * (mycols[i].overlap)) * 7.0f;
				collisionpos = entonetrans->position - (mycols[i].projectionaxis * (mycols[i].overlap));
				entonevel->velocity = glm::vec3(0, 0, 0);
				entonevel->speed = 0;
			}
			if (!enttwocol->imstatic && enttwovel)
			{
				enttwotrans->position -= (mycols[i].projectionaxis * (mycols[i].overlap)) * 7.0f;
				collisionpos = entonetrans->position + (mycols[i].projectionaxis * (mycols[i].overlap));
				enttwovel->velocity = glm::vec3(0, 0, 0);
				enttwovel->speed = 0;
			}

			std::shared_ptr<soundComponet> newsound = std::make_shared<soundComponet>();
			newsound->mysound = physicssound;
			newsound->mytype = newsound->onetime;
			newsound->position = collisionpos;
			myworld->returnmanager()->createsound(newsound);
		}


		if (!(entonecol->imstatic && enttwocol->imstatic))
		{
			healthanddamage(entone, enttwo);
		}
	}

	root->clearcols();

	root = NULL;


}

void CollisionSystem::healthanddamage(std::shared_ptr<ACC::entity> entone, std::shared_ptr<ACC::entity> enttwo)
{
	std::shared_ptr<healthcomponent> enthealthone = entone->getcomponent<healthcomponent>();
	std::shared_ptr<healthcomponent> enthealthtwo = enttwo->getcomponent<healthcomponent>();

	std::shared_ptr<damagedelt> entonedamage = entone->getcomponent<damagedelt>();
	std::shared_ptr<damagedelt> enttwodamage = enttwo->getcomponent<damagedelt>();

	sound hitsound;
	glm::vec3 hitpos;
	bool soundtoplay = false;

	if (enthealthone != NULL)
	{
		if (enthealthone->hitdelete.first == true)
		{
			enthealthone->hitdelete.second = true;
			std::shared_ptr<transposecomponent> entonetrans = entone->getcomponent<transposecomponent>();
			hitpos = entonetrans->position;
			hitsound = entonedamage->damagesound;
			soundtoplay = true;
		}
		if (enttwodamage != NULL)
		{
			enthealthone->health -= enttwodamage->damage;
		}
	}

	if (enthealthtwo != NULL)
	{
		if (enthealthtwo->hitdelete.first == true)
		{
			enthealthtwo->hitdelete.second = true;
			std::shared_ptr<transposecomponent> enttwotrans = enttwo->getcomponent<transposecomponent>();
			hitpos = enttwotrans->position;
			hitsound = enttwodamage->damagesound;
			soundtoplay = true;
		}
		if (entonedamage != NULL)
		{
			enthealthtwo->health -= entonedamage->damage;
		}
	}

	if (soundtoplay)
	{
		std::shared_ptr<soundComponet> newsound = std::make_shared<soundComponet>();
		newsound->mysound = hitsound;
		newsound->mytype = newsound->onetime;
		newsound->position = hitpos;
		myworld->returnmanager()->createsound(newsound);
	}
	
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
