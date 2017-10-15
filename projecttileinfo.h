#pragma once

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <string>

struct projectileinfo
{
	glm::vec3 pos;
	glm::vec3 dir;
	float dam;
	glm::quat rota;
	std::string team;
	float speed;
};