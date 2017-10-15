#pragma once

#include "glew.h"
#include <glm.hpp>
#include <string>

struct shadersettings
{
private:

	std::string mytype;

public:

	virtual void setupshader(GLuint program)
	{
		return;
	}
	
	std::string returnmytype()
	{
		return mytype;
	}

	void setmytype(std::string type)
	{
		mytype = type;
	}

};