#include "Teamunits.h"

void teamunits::removeunit(std::shared_ptr<ACC::entity> toremove)
{
	for (int i = 0; i < allunits.size(); i++)
	{
		if (allunits[i] == toremove)
		{
			allunits.erase(allunits.begin() + i);
			break;
		}
	}

	if (toremove->returntype() == enttypes::Fighter)
	{
		for (int i = 0; i < fighters.size(); i++)
		{
			if (fighters[i] == toremove)
			{
				fighters.erase(fighters.begin() + i);
				return;
			}
		}
	}
	if (toremove->returntype() == enttypes::Bomber)
	{
		for (int i = 0; i < bomber.size(); i++)
		{
			if (bomber[i] == toremove)
			{
				bomber.erase(bomber.begin() + i);
				return;
			}
		}
	}
	if (toremove->returntype() == enttypes::Turrent)
	{
		for (int i = 0; i < turrets.size(); i++)
		{
			if (turrets[i] == toremove)
			{
				turrets.erase(turrets.begin() + i);
				return;
			}
		}
	}
	if (toremove == capital)
	{
		capital = NULL;
		return;
	}
	if (player == toremove)
	{
		player = NULL;
		return;
	}
}
