#include "AABB.h"

void AABB::setupabb(std::vector<mesh> meshes) //setup for AABB
{
	glm::vec3 tempmin = glm::vec3(1000,1000,1000); 
	glm::vec3 tempmax = glm::vec3(-1000, -1000, -1000);

	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].vertices.size(); j++)
		{
			if (tempmin.x > meshes[i].vertices[j].vertexpos.x)
			{
				tempmin.x = meshes[i].vertices[j].vertexpos.x;
			}
			if (tempmin.y > meshes[i].vertices[j].vertexpos.y)
			{
				tempmin.y = meshes[i].vertices[j].vertexpos.y;
			}
			if (tempmin.z > meshes[i].vertices[j].vertexpos.z)
			{
				tempmin.z = meshes[i].vertices[j].vertexpos.z;
			}
			if (tempmax.x <meshes[i].vertices[j].vertexpos.x)
			{
				tempmax.x = meshes[i].vertices[j].vertexpos.x;
			}
			if (tempmax.y < meshes[i].vertices[j].vertexpos.y)
			{
				tempmax.y = meshes[i].vertices[j].vertexpos.y;
			}
			if (tempmax.z < meshes[i].vertices[j].vertexpos.z)
			{
				tempmax.z = meshes[i].vertices[j].vertexpos.z;
			}
		}
	}

	min = tempmin + glm::vec3(-0.5, -0.5, -0.5); //offset for bigger box
	max = tempmax + glm::vec3(0.5, 0.5, 0.5);
	newmin = min;
	newmax = max;

}

void AABB::update(glm::quat rotation, glm::vec3 pos) //update our col data
{
	mypos = pos;

	newmin = rotation * min;
	newmax = rotation * max;

	if (newmin.x > newmax.x) //need to double check as rotation can have these effects
	{
		float temp = newmax.x;
		newmax.x = newmin.x;
		newmin.x = temp;
	}
	if (newmin.y > newmax.y)
	{
		float temp = newmax.y;
		newmax.y = newmin.y;
		newmin.y = temp;
	}
	if (newmin.z > newmax.z)
	{
		float temp = newmax.z;
		newmax.z = newmin.z;
		newmin.z = temp;
	}

	newmin += pos;
	newmax += pos;


}

bool AABB::collision(std::shared_ptr<AABB> otherABB)
{


	glm::vec3 othermax = otherABB->returnmax();
	glm::vec3 othermin = otherABB->returnmin();


	if (newmin.y > othermax.y) //we assume there is a collision and try and test to see if there is not (this is an easier methord then trying to test is there is) 
	{
		return false;
	}
	else if (othermin.y > newmax.y)
	{
		return false;
	}
	else if (newmin.x > othermax.x)
	{
		return false;
	}
	else if (othermin.x > newmax.x)
	{
		return false;
	}
	else if (newmin.z > othermax.z)
	{
		return false;
	}
	else if (othermin.z > newmax.z)
	{
		return false;
	}


	return true;
}

glm::vec3 AABB::returnmax()
{
	return newmax;
}

glm::vec3 AABB::returnmin()
{
	return newmin;
}

bool AABB::raycollision(Ray testray, float &t)
{
	float far = 1.0;
	float near = 0.0;

	glm::vec3 farraypoint = testray.pos + (testray.dir * 1000000.0f);

	glm::vec3 toray = farraypoint - testray.pos;

	for (int i = 0; i < 3; i++)
	{
		float Fnear = (newmin[i] - testray.pos[i]) / (farraypoint[i] - testray.pos[i]);
		float Ffar = (newmax[i] - testray.pos[i]) / (farraypoint[i] - testray.pos[i]);

		if (Ffar < Fnear) // swap the sides around if needed
		{
			float temp = Ffar;
			Ffar = Fnear;
			Fnear = temp;
		}

		far = glm::min(far, Ffar);
		near = glm::max(Fnear, near);

		if (far < near)
		{
			return false; // just a safty check
		}

	}

	if (near < t)
	{
		t = near;
	}

	/*float interection = glm::length((testray.pos + toray * near) - testray.pos);
	if (interection < t)
	{
		t = interection;
	}*/

	return true; 



}
