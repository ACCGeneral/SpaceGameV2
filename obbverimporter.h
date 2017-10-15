#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "OBB.h"


class obbverimporter
{

private:
	std::vector<OBB> OBBs;


public:

	obbverimporter();

	std::vector<OBB> returnloadeddata();

	void loadobbdata(std::string filename);


};