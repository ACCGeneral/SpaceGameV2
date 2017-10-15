#include "entity.h"
#include "component.h"


ACC::entity::entity(unsigned long long int myid, enttypes::entitytype me) //set up entity with id
{
	id = myid;
	deleteme = false;
	mytype = me;

}

void ACC::entity::addcomponent(std::shared_ptr<ACC::component> comp) // add a component to the entity
{
	mycomponents.push_back(comp);


}


void ACC::entity::deletecomponent(int id) // delete a component of the entity
{
	int i = 0;

	for (auto num : mycomponents)
	{
		if (num->GetType() == id)
		{
			mycomponents.erase(mycomponents.begin() + i);
			return;
		}

		i++;
	}

}

std::vector<std::shared_ptr<ACC::component>> ACC::entity::getAllcomponets() // return the vector of components
{
	return mycomponents;
}


int ACC::entity::returnID() //return entity id
{
	return id;
}

bool ACC::entity::returndeleteme() // return delete flag
{
	return deleteme;
}

void ACC::entity::cleanup() // clean up all of the components i have and the data
{

}

void ACC::entity::setdeleteme() // set the delete flag
{
	deleteme = true;
}


bool ACC::entity::doihave(std::vector<unsigned long int> componentIDs) //check to see if a system wants the entity with a list of components 
{
	for (int i = 0; i < mycomponents.size(); i++)
	{
		for (int j = 0; j < componentIDs.size(); j++)
		{
			if (componentIDs[j] == mycomponents[i]->GetType())
			{
				componentIDs.erase(componentIDs.begin() + j);
			}
		}
	}

	if (componentIDs.size() == 0)
	{
		return true;
	}


	return false;
}

bool ACC::entity::doihave(unsigned long int componentIDs) //check to see if a system wants the entity with a single components 
{
	for (int i = 0; i < mycomponents.size(); i++)
	{
		if (mycomponents[i]->GetType() == componentIDs)
		{
			return true;
		}
	}

	return false;
}