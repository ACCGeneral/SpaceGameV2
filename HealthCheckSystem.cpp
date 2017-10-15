#include "HealthCheckSystem.h"
#include "OctTree.h"

HealthCheckSystem::HealthCheckSystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "HealthCheckSystem";
}

void HealthCheckSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(healthcomponent::TypeID))
		{
			healthobjects.push_back(ent[i]);
		}
	}

}

void HealthCheckSystem::update(float & dt, bool & go)
{
	OctTree *root = myworld->returnoct();
	bool removefrom = false;

	for (auto num : healthobjects)
	{
		std::shared_ptr<healthcomponent> healthobjects = num->getcomponent<healthcomponent>();
		removefrom = false;

		if (healthobjects->health <= 0 || healthobjects->hitdelete.second == true)
		{
			myworld->returnmanager()->addtodeletelist(num);
			root->removethis(num->returnID(), removefrom);

			std::shared_ptr<thingsiown> mythings = num->getcomponent<thingsiown>();
			if (mythings != NULL)
			{
				for (int i = 0; i < mythings->stuffiown.size(); i++)
				{
					myworld->returnmanager()->addtodeletelist(mythings->stuffiown[i]);
				}
			}
		}
	}
}

void HealthCheckSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{

	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < healthobjects.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (healthobjects[i] == ents[j])
			{
				healthobjects.erase(healthobjects.begin() + i);
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
