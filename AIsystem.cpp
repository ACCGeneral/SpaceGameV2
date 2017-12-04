#include "AIsystem.h"

AIsystem::AIsystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w,EventSys)
{
	my_System_Name = "AIsystem";

}

void AIsystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(AIcomp::TypeID))
		{
			AIobjects.push_back(ent[i]);
			std::shared_ptr<teamcomponent> myteamcomp = ent[i]->getcomponent<teamcomponent>();

			if (ent[i]->returntype() == enttypes::Capital && !myteamcomp->ally)
			{
				AIman.addenemycap(ent[i]);
			}
			else if (ent[i]->returntype() == enttypes::Capital && myteamcomp->ally)
			{
				AIman.addallycap(ent[i]);
			}
			else
			{
				if (myteamcomp->ally)
				{
					AIman.addtoteamone(ent[i]);
				}
				else
				{
					AIman.addtoteamttwo(ent[i]);
				}
			}

		}
		else if (ent[i]->doihave(playercomp::TypeID))
		{
			AIobjects.push_back(ent[i]);
			AIman.addtoteamone(ent[i]);
		}
	}
}

void AIsystem::update(float & dt, bool & go)
{
	AIman.update(dt,myworld);
}

void AIsystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < AIobjects.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (AIobjects[i] == ents[j])
			{
				std::cout << "dead \n";
				AIman.removefromlist(AIobjects[i]);
				AIobjects.erase(AIobjects.begin() + i);
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

