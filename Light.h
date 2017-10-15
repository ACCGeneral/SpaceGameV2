#pragma once

#include "glew.h"
#include <glm.hpp>
#include <string>

enum Lighttype {DLight,PLight,Slight}; // all our light types


//base class for all our lights
class light
{

protected:

	Lighttype mylight;

public:


	Lighttype returnlighttype()
	{
		return mylight;
	}

	virtual void shaderlinks(GLuint program, std::string index) = 0;

};
