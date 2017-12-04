#pragma once

#include "Events.h"
#include <vector>

class EventSystem
{
private:

	std::vector<std::shared_ptr<Event>> myevents;

public:

	std::vector<std::shared_ptr<Event>> getmyevents()
	{
		return myevents;
	}

	void setevents(std::vector<std::shared_ptr<Event>> neweventset)
	{
		myevents = neweventset;
	}

	void setevent(std::shared_ptr<Event> newevent)
	{
		myevents.push_back(newevent);
	}
};
