#include "CollisionResolveSystem.h"
#include "OctTree.h"
#include "Octinfo.h"

CollisionResolveSystem::CollisionResolveSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "CollisionResolveSystem";
	physicssound = Resourecmanager::instance().getsound("Collision");
}

void CollisionResolveSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{


}

void CollisionResolveSystem::update(float & dt, bool & go)
{
	bool removefrom = false;
	OctTree *root = myworld->returnoct();

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
			glm::vec3 collisionpos = glm::vec3(0, 0, 0);

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
			if (entone->doihave(healthcomponent::TypeID))
			{
				std::shared_ptr<healthevenet> newdamageevent = std::make_shared<healthevenet>();
				newdamageevent->takingdamageID = entone->returnID();
				newdamageevent->position = entonetrans->position;
				if (enttwo->doihave(damagedelt::TypeID))
				{
					newdamageevent->damagedealerID = enttwo->returnID();
				}
				myEventsSystem->setevent(newdamageevent);
			}
			if (enttwo->doihave(healthcomponent::TypeID))
			{
				std::shared_ptr<healthevenet> newdamageevent = std::make_shared<healthevenet>();
				newdamageevent->takingdamageID = enttwo->returnID();
				newdamageevent->position = enttwotrans->position;
				if (entone->doihave(damagedelt::TypeID))
				{
					newdamageevent->damagedealerID = entone->returnID();
				}
				myEventsSystem->setevent(newdamageevent);
			}
		}
	}

	root->clearcols();

	root = NULL;

}

void CollisionResolveSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{



}


