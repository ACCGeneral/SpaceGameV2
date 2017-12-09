#include "ParticleSystem.h"
#include "maincamera.h"
#include "particleSphereGen.h"
#include "PointLight.h"

ParticleSystem::ParticleSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "ParticleSystem";

}

void ParticleSystem::newEmitterEvent()
{

	std::vector<std::shared_ptr<Event>> allevents = myEventsSystem->getmyevents();

	for (int i = 0; i < allevents.size(); i++)
	{
		if (allevents[i]->mytype == explotion)
		{
			std::shared_ptr<explostionEvent> newEmitterEvent = std::static_pointer_cast<explostionEvent>(allevents[i]);
			std::shared_ptr<pointlight> newPointInfo = std::make_shared<pointlight>();
			newPointInfo->setpos(newEmitterEvent->explotionData.pos);
			newPointInfo->setamb(glm::vec3(1.0, 0.8, 0.8));
			newPointInfo->setlightcolour(glm::vec3(1.0, 0.8, 0.8));
			newPointInfo->setconstant(1.0f);
			newPointInfo->setlinear(0.045);
			newPointInfo->setquadratic(0.0075);
			myworld->returnmanager()->createExplotionParticleEffect(newEmitterEvent->explotionData, newPointInfo);
			allevents.erase(allevents.begin() + i);
			i--;
		}
	}
	myEventsSystem->setevents(allevents);
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

	newEmitterEvent();

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
			glm::vec3 up = glm::vec3(0, 1, 0) * quadcamera;
			glm::vec3 lookat = myworld->getmaincam()->getpos() + myworld->getmaincam()->getdir();
			particleEmit->particleemitter->SetMatrices(myworld->getmaincam()->getproj(), myworld->getmaincam()->getpos(), lookat, up); //bug here
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
