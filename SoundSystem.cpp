#include "SoundSystem.h"

SoundSystem::SoundSystem(std::shared_ptr<world> w, float vol, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "SoundSystem";
	volume = vol;

}

void SoundSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(soundComponet::TypeID))
		{
			soundobjects.push_back(ent[i]);
			std::shared_ptr<soundComponet> mycomp = ent[i]->getcomponent<soundComponet>();
			mycomp->mysound.changevol(volume);
			mycomp->mysound.playsound();
		}
	}
}

void SoundSystem::update(float & dt, bool & go)
{
	for (int i = 0; i < soundobjects.size(); i++)
	{
		std::shared_ptr<soundComponet> mycomp = soundobjects[i]->getcomponent<soundComponet>();
		if (mycomp->mysound.isStoped())
		{
			if (mycomp->mytype == mycomp->onetime)
			{
				myworld->returnmanager()->addtodeletelist(soundobjects[i]);
			}
			else  if (mycomp->mytype == mycomp->repeate)
			{
				mycomp->mysound.playsound();
			}
		}
	}
}

void SoundSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < soundobjects.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (soundobjects[i] == ents[j])
			{
				std::shared_ptr<soundComponet> mycomp = soundobjects[i]->getcomponent<soundComponet>();
				mycomp->mysound.cleanup();
				soundobjects.erase(soundobjects.begin() + i);
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
