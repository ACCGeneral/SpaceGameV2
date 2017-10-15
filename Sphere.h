#pragma once

#include <glm.hpp>
#include "Ray.h"

struct sphere
{
	glm::vec3 center;
	float rad;
	glm::vec3 newpos;

	sphere()
	{

	}
	sphere(glm::vec3 min, glm::vec3 max);
	void update(glm::vec3 pos);
	bool raycollision(Ray testray, float &t);


};