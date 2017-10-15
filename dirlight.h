#pragma once
#include "Light.h"

// the direction light class based of the light class
class dirlight : public light
{

private:

	glm::vec3 direction;
	glm::vec3 Ambient;
	glm::vec3 lightcolour;

	std::string ambatt;
	std::string diratt;
	std::string lightatt;

public:

	dirlight()
	{
		mylight = DLight;
		ambatt = "ambient";
		diratt = "direction";
		lightatt = "lightcolour";
	}

	void shaderlinks(GLuint program, std::string index);


	void setdir(glm::vec3 dir)
	{
		direction = dir;
	}
	void setamb(glm::vec3 amb)
	{
		Ambient = amb;
	}
	void setlightcolour(glm::vec3 LC)
	{
		lightcolour = LC;
	}


};
