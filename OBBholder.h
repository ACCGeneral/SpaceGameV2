#pragma once
#include <vector>
#include <string>
#include "obbverimporter.h"
#include "OBB.h"

class OBBholder
{
private:

	obbverimporter myimporter;
	std::vector<std::pair<std::vector<OBB>, std::string>> OBBmodel;

public:

	OBBholder();
	~OBBholder();

	void addOBB(std::string OBBpath, std::string OBBname);
	std::vector<OBB> getOBB(std::string name);

};
