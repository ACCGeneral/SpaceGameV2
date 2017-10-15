#pragma once

#include "System.h"
#include <vector>
#include <memory>
#include "world.h"
// our system manager that will be in charge or running all of our systems as well as cleanup

class scene;

namespace ACC
{

	class System_Manager
	{

	private:

		std::vector<std::shared_ptr<System>> A_Systems;
		std::shared_ptr<System> rendersystem;
		std::shared_ptr<scene> myscene;
		std::shared_ptr<world> myworld;


	public:

		System_Manager(std::shared_ptr<scene> s, std::shared_ptr<world> w);

		void updatesystements();
		void deletesystements();
		void addsystem(std::shared_ptr<System> system);
		void update(float dt, bool &go);
		void render(float dt, bool &go);
		bool cleanupsystems();
		void setrender(std::shared_ptr<System> rend);
	};

}