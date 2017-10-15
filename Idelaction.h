#pragma once

#include "action.h"
#include "component.h"


class idelaction : public action
{
private:


public:

	idelaction()
	{
		mytype = "mytype";
	}

	void start();
	void run(float dt, std::shared_ptr<world> myworld);
	void end();

};