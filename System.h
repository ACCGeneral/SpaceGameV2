#pragma once

#include <string>
#include "entitymanager.h"
#include <gtc/matrix_transform.hpp> 
#include <gtx/matrix_interpolation.hpp>
#include <math.h>
#include "inputlogger.h"
#include "world.h"
#include <gtc/type_ptr.hpp>
#include <typeinfo>
#include "EventSystem.h"

// here we have the base system class and the other systems we need for the assignment

namespace ACC
{
	class System
	{
	protected:

		std::shared_ptr<world> myworld;
		std::shared_ptr<EventSystem> myEventsSystem;

		std::string my_System_Name;
	public:

		System(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys)
		{
			myEventsSystem = EventSys;
			myworld = w;
		}

		virtual void soiwant(std::vector<std::shared_ptr<ACC::entity>> ent) = 0;

		virtual void update(float &dt, bool &go) = 0;

		virtual std::string returnname() = 0;

		virtual void doihave(std::vector<std::shared_ptr<ACC::entity>> ent) = 0;

		virtual void cleanup() = 0;

	};

}




