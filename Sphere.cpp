#include "Sphere.h"

sphere::sphere(glm::vec3 min, glm::vec3 max)
{

	center.x = ((max.x - min.x) / 2) + min.x;
	center.y = ((max.y - min.y) / 2) + min.y;
	center.z = ((max.z - min.z) / 2) + min.z;

	float maxrad = 0;

	maxrad = std::fmax((max.x - center.x),center.x - min.x);
	maxrad = std::fmax(std::fmax((max.y - center.y), center.y - min.y),maxrad);
	maxrad = std::fmax(std::fmax((max.z - center.z), center.z - min.z), maxrad);

	rad = maxrad * 1.2;
}

void sphere::update(glm::vec3 pos)
{
	newpos = pos + center;
}

bool sphere::raycollision(Ray testray, float &t)
{
	glm::vec3 m = testray.pos - newpos;

	float b = glm::dot(m, testray.dir);
	float c = glm::dot(m, m) - rad * rad;

	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float deter = b*b - c;

	if (deter < 0.0)
	{
		return false;
	}

	t = -b - glm::sqrt(deter);

	if (t < 0.0)
	{
		t = 0;
	}


	return true;
}
