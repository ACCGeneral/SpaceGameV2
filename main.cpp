#include <iostream>
#include "engine.h"
#include "entitymanager.h"
#include <memory>

// our main where we create and setup an instance of our engine
int main(int argc, char *argv[])
{
	std::shared_ptr<ACC::engine> myengine = std::make_shared<ACC::engine>(); //create engine instance 

	bool setupcheck = myengine->OpenglSdlSetup(1280, 720);

	if (setupcheck == false)
	{
		return -1;
	}

	myengine->init();

	while (myengine->go)
	{
		myengine->update();
	}


	myengine->cleanup(); //final cleanup
	

	return 0;
}