#pragma once

#include <string>
#include <vector>

#include "glm.hpp"

struct maneuver
{
	std::vector<glm::vec3> manpos;
	std::vector<float> roll;
	std::vector<float> time;
	float fulltime;
};

class maneuvers
{
private:

	std::vector<std::pair<maneuver, std::string>> mymaneuver;

public:

	maneuvers();

	maneuver getrandom();
	maneuver getcurtain(std::string man);
	void addman(std::vector<glm::vec3> pos, std::vector<float> r, std::vector<float> t, std::string name);

};