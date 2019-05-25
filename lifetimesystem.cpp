#include "lifetimesystem.h"
#include "OctTree.h"

lifetimesystem::lifetimesystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "lifetimesystem";
	root = myworld->returnoct();
}

void lifetimesystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(lifespan::TypeID))
		{
			lifeents.push_back(ent[i]);
		}
	}

}

void lifetimesystem::update(float & dt, bool & go)
{
	bool removefrom = false;

	for (auto ent : lifeents)
	{
		removefrom = false;
		std::shared_ptr<lifespan> entslifespan = ent->getcomponent<lifespan>();
		entslifespan->lifetime -= dt;

		if (entslifespan->lifetime <= 0 && !ent->returndeleteme())
		{
			myworld->returnmanager()->addtodeletelist(ent);
			bool removefrom = false;
			root->removethis(ent->returnID(), removefrom);
		}
	}
}

void lifetimesystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < lifeents.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (lifeents[i] == ents[j])
			{
				lifeents.erase(lifeents.begin() + i);
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
