#pragma once

#include <vector>
#include <string>
#include <typeinfo>
#include "component.h"
#include "EntityTypes.h"

//our entity class which every game object will be based on
namespace ACC
{

	class entity
	{
		unsigned long long int id;
		std::vector<std::shared_ptr<ACC::component>> mycomponents;
		bool deleteme;
		enttypes::entitytype mytype;

	public:

		entity(unsigned long long int myid, enttypes::entitytype me);
		void addcomponent(std::shared_ptr<ACC::component> comp);
		void deletecomponent(int id);
		std::vector<std::shared_ptr<ACC::component>> getAllcomponets();
		int returnID();
		bool returndeleteme();
		void cleanup();
		void setdeleteme();

		enttypes::entitytype returntype()
		{
			return mytype;
		}
		

		template <typename Type>  //a template so we dont need a function for every component
		std::shared_ptr<Type> getcomponent()
		{
			for (auto num : mycomponents)
			{
				if (num->GetType() == Type::TypeID)
				{
					return std::static_pointer_cast<Type>(num); //return the component we need 
				}

			}
			return NULL;
		}

		bool doihave(std::vector<unsigned long int> componentIDs);
		bool doihave(unsigned long int componentIDs);

	};
}