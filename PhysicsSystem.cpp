#include "PhysicsSystem.h"
#include "maincamera.h"

PhysicsSystsem::PhysicsSystsem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "PhysicsSystsem";

}

void PhysicsSystsem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(physics::TypeID))
		{
			physicsobjects.push_back(ent[i]);
		}
	}

}

void PhysicsSystsem::update(float & dt, bool & go)
{
	for (auto ent : physicsobjects)
	{
		std::shared_ptr<physics> entvel = ent->getcomponent<physics>();
		std::shared_ptr<transposecomponent> enttra = ent->getcomponent<transposecomponent>();

		float test = glm::length(entvel->velocity);

		if (entvel->physicsobject == false)
		{
			enttra->position += entvel->velocity * dt;
		}
		else
		{
			if (glm::length(entvel->force) > entvel->maxforce)
			{
				entvel->force = glm::normalize(entvel->force) * entvel->maxforce;
			}
			glm::vec3 acceleration = entvel->force / (entvel->mass);
			entvel->force = glm::vec3(0.0f, 0.0f, 0.0f);
			entvel->velocity += acceleration * dt;
			if (glm::length(entvel->velocity) > entvel->maxspeed)
			{
				entvel->velocity = glm::normalize(entvel->velocity) * entvel->maxspeed;
			}
			enttra->position += entvel->velocity * dt;
		}
	}
	myworld->getmaincam()->setView();
}

void PhysicsSystsem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < physicsobjects.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (physicsobjects[i] == ents[j])
			{
				physicsobjects.erase(physicsobjects.begin() + i);
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


