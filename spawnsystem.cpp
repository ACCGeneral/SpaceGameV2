#include "spawnsystem.h"
#include "entitymanager.h"
#include "entity.h"

spawnsystem::spawnsystem()
{
	allyspawnnum = 0;
	enemyspawnnum = 0;


	allyspawns = std::vector<glm::vec3>{ glm::vec3(650,10,10), glm::vec3(650,20,60), glm::vec3(680,-20,10),glm::vec3(680,-30,-10), glm::vec3(680,30,-10),glm::vec3(600,30,0), glm::vec3(610,30,10) };
	enemyspawns = std::vector<glm::vec3>{ glm::vec3(-650,10,110), glm::vec3(-650,20,160), glm::vec3(-680,-20,110),glm::vec3(-680,-30,90), glm::vec3(-680,30,90),glm::vec3(-600,30,100), glm::vec3(-610,30,110) };
}

glm::vec3 spawnsystem::getallyspawn()
{
	int returnnum = allyspawnnum;
	allyspawnnum++;

	if (allyspawnnum > allyspawns.size() - 1)
	{
		allyspawnnum = 0;
	}

	if (returnnum < allyspawns.size())
	{
		return allyspawns[returnnum];
	}

	return glm::vec3(0, 0, 0);

	
}

glm::vec3 spawnsystem::getenemyspawn()
{
	int returnnum = enemyspawnnum;
	enemyspawnnum++;

	if (enemyspawnnum > enemyspawns.size() - 1)
	{
		enemyspawnnum = 0;
	}

	if (returnnum < enemyspawns.size())
	{
		return enemyspawns[returnnum];
	}

	return glm::vec3(0, 0, 0);
}

void spawnsystem::createents(std::shared_ptr<entitymanager> mymanger)
{
	if (myspawndata.size() > 0)
	{
		for (int i = 0; i < myspawndata.size(); i++)
		{
			if (myspawndata[i].countdown <= 0)
			{
				if (myspawndata[i].type == "player")
				{
					mymanger->createplayer(getallyspawn());
					myspawndata.erase(myspawndata.begin() + i);
					i--;
				}
				else if (myspawndata[i].type == "allyfighter")
				{
					mymanger->createAllyFighter(getallyspawn());
					myspawndata.erase(myspawndata.begin() + i);
					i--;
				}
				else if (myspawndata[i].type == "allybomber")
				{
					mymanger->createAllyBomber(getallyspawn());
					myspawndata.erase(myspawndata.begin() + i);
					i--;
				}
				else if (myspawndata[i].type == "enemyfighter")
				{
					mymanger->createEnemyFighter(getenemyspawn());
					myspawndata.erase(myspawndata.begin() + i);
					i--;
				}
				else if (myspawndata[i].type == "enemybomber")
				{
					mymanger->createEnemyBomber(getenemyspawn());
					myspawndata.erase(myspawndata.begin() + i);
					i--;
				}

			}
		}
	}

}

void spawnsystem::addrespawndata(std::vector<std::shared_ptr<ACC::entity>> deadends)
{
	for (auto ent : deadends)
	{
		std::shared_ptr<teamcomponent> myteam = ent->getcomponent<teamcomponent>();

		if (ent->returntype() == enttypes::Player)
		{
			spawndata newspawn;
			newspawn.countdown = 5;
			newspawn.type = "player";
			myspawndata.push_back(newspawn);
			std::cout << "player had died adding entry to create new one \n";
		}
		else if (ent->returntype() == enttypes::Fighter)
		{
			spawndata newspawn;
			newspawn.countdown = 7;
			if (myteam->ally)
			{
				newspawn.type = "allyfighter";
			}
			else
			{
				newspawn.type = "enemyfighter";
			}
			myspawndata.push_back(newspawn);
		}
		else if (ent->returntype() == enttypes::Bomber)
		{
			spawndata newspawn;
			newspawn.countdown = 7;
			if (myteam->ally)
			{
				newspawn.type = "allybomber";
			}
			else
			{
				newspawn.type = "enemybomber";
			}
			myspawndata.push_back(newspawn);
		}
	}
	
}

void spawnsystem::update(float dt)
{
	if (myspawndata.size() > 0)
	{
		for (int i = 0; i < myspawndata.size(); i++)
		{
			myspawndata[i].countdown -= dt;
		}
	}

}
