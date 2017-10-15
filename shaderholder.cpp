#include "shaderholder.h"

shaderholder::shaderholder()
{


}

void shaderholder::addshader(std::shared_ptr<shader> newshader, std::string shadername) //store a new shader to our shader list
{
	newshader->linkshader();
	bool test = newshader->linkcheck();
	if (test == false)
	{
		std::cout << "problem with shader names " + shadername;
	}
	else
	{
		std::pair<std::shared_ptr<shader>, std::string> toadd;
		toadd.first = newshader;
		toadd.second = shadername;
		shaders.push_back(toadd);
	}

}

std::shared_ptr<shader> shaderholder::getshader(std::string name) //get the shader tht we want 
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (shaders[i].second == name)
		{
			return shaders[i].first;
		}
	}


	return NULL;
}





