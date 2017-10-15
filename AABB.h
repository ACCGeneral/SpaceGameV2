#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "mesh.h"
#include "Ray.h"

//class for aabb collision detection

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 newmin;
	glm::vec3 newmax;
	
	glm::vec3 mypos;

	void setupabb(std::vector<mesh> meshes);
	void update(glm::quat rotation, glm::vec3 pos);
	bool collision(std::shared_ptr<AABB> otherABB);

	glm::vec3 returnmax();
	glm::vec3 returnmin();

	bool raycollision(Ray testray, float &t);

};