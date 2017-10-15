#include "OBBholder.h"

OBBholder::OBBholder()
{


}

OBBholder::~OBBholder()
{


}

void OBBholder::addOBB(std::string OBBpath, std::string OBBname)
{
	myimporter.loadobbdata(OBBpath);
	std::pair<std::vector<OBB>, std::string> newpair;
	newpair.first = myimporter.returnloadeddata();
	newpair.second = OBBname;
	OBBmodel.push_back(newpair);

}

std::vector<OBB> OBBholder::getOBB(std::string name)
{
	for (int i = 0; i < OBBmodel.size(); i++)
	{
		if (OBBmodel[i].second == name)
		{
			return OBBmodel[i].first;
		}
	}

	std::vector<OBB> blank;

	return blank;
}
