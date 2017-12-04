#pragma once

#include <glm.hpp>

struct particleExplotionData
{
	glm::vec3 pos;
	float radious;
	float minLife;
	float maxAddedLife;
	float particleSize;
	float spawnEvery;
	int numberToGen;
	int maxCycles;

	int generators;
};