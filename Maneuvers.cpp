#include "Maneuvers.h"
#include <iostream>

maneuvers::maneuvers()
{
	maneuver lefttoright;
	lefttoright.manpos = std::vector<glm::vec3>{ glm::vec3(0.7,0.3,1),glm::vec3(-0.7, -0.3, 1),glm::vec3(0.7, 0, 1) };
	lefttoright.roll = std::vector<float>{ 90.0f,-90.0f,0.0f };
	lefttoright.time = std::vector<float>{ 5.0f,5.0f,5.0f };
	lefttoright.fulltime = 5.0f + 5.0f + 5.0f;

	std::pair<maneuver, std::string> newpair;
	newpair.first = lefttoright;
	newpair.second = "lefttoright";
	mymaneuver.push_back(newpair);
	bombermaneuvers.push_back(newpair);

	maneuver ssplit;
	ssplit.manpos = std::vector<glm::vec3>{ glm::vec3(0, 0, 1),glm::vec3(0, -0.5, 0.3),glm::vec3(0, -0.2, 0.1), glm::vec3(0, -0.2, -0.3), glm::vec3(0, -0.2, -0.5),glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)};
	ssplit.roll = std::vector<float>{ 90.0f,180.0f,270.0f,360.0f,360.0f,360.0f,360.0f };
	ssplit.time = std::vector<float>{ 1.0f,3.0f,3.0f,3.0f,3.0f,3.0f,1.0f};
	ssplit.fulltime = 1.0f + 3.0f + 3.0f + 3.0f + 3.0f + 3.0f + 1.0f;
	newpair.first = ssplit;
	newpair.second = "ssplit";
	mymaneuver.push_back(newpair);
	bombermaneuvers.push_back(newpair);

	maneuver loop;
	loop.manpos = std::vector<glm::vec3>{ glm::vec3(0, 1, 0), glm::vec3(0, -0.3, -0.7),glm::vec3(0, -0.3, 0.7), glm::vec3(0, -0.3, 0.7),glm::vec3(0, 0, 1)};
	loop.roll = std::vector<float>{0.0f,0.0f,0.0f,0.0f,0.0f};
	loop.time = std::vector<float>{ 3.0f,3.0f,3.0f,3.0f,2.0f};
	loop.fulltime = 3.0f + 3.0f + 3.0f + 3.0f + 2.0f;
	newpair.first = loop;
	newpair.second = "loop";
	mymaneuver.push_back(newpair);
	bombermaneuvers.push_back(newpair);

	maneuver smalllefttoright;
	smalllefttoright.manpos = std::vector<glm::vec3>{glm::vec3(1.0,0.2,1.0),glm::vec3(-1.0,-0.2,1.0) };
	smalllefttoright.roll = std::vector<float>{ 90.0f,270.0f};
	smalllefttoright.time = std::vector<float>{2.5f,2.5f};
	smalllefttoright.fulltime = 2.5f + 2.5f;
	newpair.first = smalllefttoright;
	newpair.second = "smalllefttoright";
	mymaneuver.push_back(newpair);

	maneuver attackrun;
	attackrun.manpos = std::vector<glm::vec3>{ glm::vec3(0.3,0.0,1.0), glm::vec3(0,0,1),glm::vec3(-0.3,0,1.0), glm::vec3(0,0,1)};
	attackrun.roll = std::vector<float>{ 90.0f, 90.0f, 180.0f, 180.0f};
	attackrun.time = std::vector<float>{ 4.0f, 4.0f, 4.0f, 4.0f};
	attackrun.fulltime = 4.0f + 4.0f + 4.0f, + 4.0f;
	newpair.first = attackrun;
	newpair.second = "attackrun";
	mymaneuver.push_back(newpair);
	bombermaneuvers.push_back(newpair);
}

maneuver maneuvers::getrandom()
{
	int randomnumber = rand() % mymaneuver.size();

	return mymaneuver[randomnumber].first;
}

maneuver maneuvers::getrandombomber()
{
	int randomnumber = rand() % bombermaneuvers.size();

	return bombermaneuvers[randomnumber].first;
}

maneuver maneuvers::getcurtain(std::string man)
{
	for (int i = 0; i < mymaneuver.size(); i++)
	{
		if (mymaneuver[i].second == man)
		{
			return mymaneuver[i].first;
		}
	}
}

void maneuvers::addman(std::vector<glm::vec3> pos, std::vector<float> r, std::vector<float> t, std::string name)
{
	maneuver newman;
	newman.manpos = pos;
	newman.roll = r;
	newman.time = t;

	std::pair<maneuver, std::string> newpair;
	newpair.first = newman;
	newpair.second = name;
	mymaneuver.push_back(newpair);

}
