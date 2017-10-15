#include "obbverimporter.h"

obbverimporter::obbverimporter()
{


}

std::vector<OBB> obbverimporter::returnloadeddata()
{
	return OBBs;
}

void obbverimporter::loadobbdata(std::string filename)
{
	OBBs.clear();
	std::vector<std::vector<glm::vec3>> obbpoints;

	FILE * tempfile = fopen(filename.c_str(), "r");

	if (tempfile == NULL)
	{
		std::cout << "could not open file";
		return;
	}

	std::vector<glm::vec3> obbver;

	while (1)
	{
		char info[100];
		int scan = fscanf(tempfile, "%s", info);
		if (scan == EOF)
		{
			obbpoints.push_back(obbver);
			obbver.clear();
			break;
		}

		if (strcmp(info, "v") == 0)//if wat we read is a v then we will have found a vertex
		{
			glm::vec3 tempver;
			fscanf(tempfile, "%f %f %f\n", &tempver.x, &tempver.y, &tempver.z); //look for the 3 floating point numbers 
			obbver.push_back(tempver);
		}

		if (strcmp(info, "|") == 0)
		{
			obbpoints.push_back(obbver);
			obbver.clear();
		}

	}


	for (int i = 0; i < obbpoints.size(); i++)
	{
		OBB newobb;
		newobb.addobbsetup(obbpoints[i]);
		OBBs.push_back(newobb);
	}


}
