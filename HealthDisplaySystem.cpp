#include "HealthDisplaySystem.h"

HealthDisplaySystem::HealthDisplaySystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "HealthDisplaySystem";
}

void HealthDisplaySystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(unithealthbar::TypeID))
		{
			healthobjects.push_back(ent[i]);
		}
		else if (ent[i]->doihave(playercomp::TypeID) && player == NULL)
		{
			player = ent[i];
		}
	}
}

void HealthDisplaySystem::update(float & dt, bool & go)
{
	if (player == NULL)
	{
		return;
	}


	std::shared_ptr<transposecomponent> playertrans = player->getcomponent<transposecomponent>();

	for (auto ent : healthobjects)
	{
		std::shared_ptr<unithealthbar> entunit = ent->getcomponent<unithealthbar>();
		std::shared_ptr<healthcomponent> healthofowned = entunit->unit->getcomponent<healthcomponent>();
		std::shared_ptr<transposecomponent> unittrans = entunit->unit->getcomponent<transposecomponent>();
		std::shared_ptr<transposecomponent> healthtrans = ent->getcomponent<transposecomponent>();
		std::shared_ptr<modelcomponent> mod = ent->getcomponent<modelcomponent>();

		if (entunit->playerhealth == false)
		{
			glm::vec3 uphealth = playertrans->myquat * glm::vec3(0, 1, 0);
			float lenghtupscale = glm::length(entunit->Doffset);
			healthtrans->position = unittrans->position + (uphealth * lenghtupscale);
		}
		else
		{
			float muti = glm::length(entunit->Doffset);
			healthtrans->position = unittrans->position + ((unittrans->myquat * glm::normalize(entunit->Doffset)) * muti);
			healthtrans->myquat = healthtrans->myquat * unittrans->myquat;
		}

		healthsettings *uiextra = static_cast<healthsettings*>(mod->extra.release());
		uiextra->setnormhealth(healthofowned->health / healthofowned->maxhp);
		mod->extra = std::make_unique<healthsettings>(*uiextra);
		delete uiextra;
	}
}

void HealthDisplaySystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < ents.size(); i++)
	{
		for (int j = 0; j < healthobjects.size(); j++)
		{
			if (healthobjects[j] == ents[i])
			{
				healthobjects.erase(healthobjects.begin() + j);
				ents.erase(ents.begin() + i);
				j--;
				i--;
				break;
			}

			if (ents[i] == player)
			{
				ents.erase(ents.begin() + i);
				i--;
				player = NULL;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}



}


