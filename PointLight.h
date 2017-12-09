#pragma once

#include "glew.h"
#include "light.h"
#include <glm.hpp>

//class for the spotlights that we might have 

class pointlight : public light
{
private:

	glm::vec3 position;
	glm::vec3 Ambient;
	glm::vec3 lightcolour;
	float constant;
	float linear;
	float quadratic;


	std::string ambatt;
	std::string Posatt;
	std::string lightatt;
	std::string constantatt;
	std::string linearatt;
	std::string quadraticatt;


public:

	pointlight()
	{
		mylight = PLight;
		ambatt = "ambient";
		Posatt = "position";
		lightatt = "lightcolour";
		constantatt = "constant";
		linearatt = "linear";
		quadraticatt = "quadratic";
	}


	void setpos(glm::vec3 pos)
	{
		position = pos;
	}
	void setamb(glm::vec3 amb)
	{
		Ambient = amb;
	}
	void setlightcolour(glm::vec3 LC)
	{
		lightcolour = LC;
	}
	void setconstant(float C)
	{
		constant = C;
	}
	void setlinear(float L)
	{
		linear = L;
	}
	void setquadratic(float L)
	{
		quadraticatt = L;
	}


	void shaderlinks(GLuint program, std::string index);

};