#include "ParticleSystem.h"
#include "maincamera.h"
#include "particleSphereGen.h"

ParticleSystem::ParticleSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "ParticleSystem";

}

void ParticleSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(particalemmiter::TypeID))
		{
			particlespawner.push_back(ent[i]);
		}
	}

}

void ParticleSystem::update(float & dt, bool & go)
{

	for (int i = 0; i < particlespawner.size(); i++)
	{
		std::shared_ptr<particalemmiter> particleEmit = particlespawner[i]->getcomponent<particalemmiter>();

		if (particleEmit->particleemitter->getfinished())
		{
			myworld->returnmanager()->addtodeletelist(particlespawner[i]);
		}
		else
		{
			glm::quat quadcamera = glm::toQuat(myworld->getmaincam()->getview());
			glm::vec3 up = quadcamera * glm::vec3(0, 1, 0);
			particleEmit->particleemitter->SetMatrices(myworld->getmaincam()->getproj(), myworld->getmaincam()->getpos(), myworld->getmaincam()->getdir(), up);
		}
	}

}

void ParticleSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < particlespawner.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (particlespawner[i] == ents[j])
			{
				std::shared_ptr<particalemmiter> particleEmit = particlespawner[i]->getcomponent<particalemmiter>();
				particleEmit->particleemitter->ReleaseParticleSystem();

				particlespawner.erase(particlespawner.begin() + i);
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
