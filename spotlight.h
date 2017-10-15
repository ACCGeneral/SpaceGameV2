#pragma once

#include "glew.h"
#include "light.h"
#include <glm.hpp>
 
//class for the spotlights that we might have 

class spotlight : public light
{
private:

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 Ambient;
	glm::vec3 lightcolour;
	float radious;
	float outerrad;
	float constant;
	float linear;
	float quadratic;


	std::string ambatt;
	std::string diratt;
	std::string Posatt;
	std::string lightatt;
	std::string radiousatt;
	std::string outerradatt;
	std::string constantatt;
	std::string linearatt;
	std::string quadraticatt;


public:

	spotlight()
	{
		mylight = Slight;
		ambatt = "ambient";
		diratt = "direction";
		Posatt = "position";
		lightatt = "lightcolour";
		radiousatt = "radius";
		outerradatt = "outerrad";
		constantatt = "constant";
		linearatt = "linear";
		quadraticatt = "quadratic";
	}


	void setpos(glm::vec3 pos)
	{
		position = pos;
	}

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
	void settouterrad(float OR)
	{
		outerrad = OR;
	}
	void setrad(float R)
	{
		radious = R;
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