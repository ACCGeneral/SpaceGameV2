#pragma once

#include "shader.h"
#include <memory>
#include <vector>

//class to hold all of our in game shaders

class shaderholder
{
private:

	std::vector<std::pair<std::shared_ptr<shader>, std::string>> shaders;

public:

	shaderholder();
	void addshader(std::shared_ptr<shader> newshader, std::string shadername);
	std::shared_ptr<shader> getshader(std::string name);
	
};