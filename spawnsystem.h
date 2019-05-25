#pragma once

#include <vector>
#include <glm.hpp>
#include <memory>
#include <string>

class entitymanager;

namespace ACC
{
	class entity;
}

struct spawndata
{
	std::string type;
	float countdown;
};


class spawnsystem
{
private:

	std::vector<glm::vec3> allyspawns;
	std::vector<glm::vec3> enemyspawns;
	
	int allyspawnnum;
	int enemyspawnnum;

	std::vector<spawndata> myspawndata;

public:

	spawnsystem();
	glm::vec3 getallyspawn();
	glm::vec3 getenemyspawn();


	void createents(std::shared_ptr<entitymanager> mymanger);
	void addrespawndata(std::vector<std::shared_ptr<ACC::entity>> deadends);

	void update(float dt);


};