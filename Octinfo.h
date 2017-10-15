#pragma once
#include "OBB.h"
#include "AABB.h"
#include <memory>
#include "Sphere.h"

// a small infomation struct for each collision entity to worry about

struct Octinfo
{
	std::vector<std::shared_ptr<OBB>> objectobb;
	std::shared_ptr<AABB> objectaabb;
	std::shared_ptr<sphere> objectsphere;
	bool cantmove;
	bool physicsobj;
	int unitID;
	std::vector<unsigned long int> cantcollide;
};
