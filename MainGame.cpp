#include "MainGame.h"
#include "maincamera.h"
#include "OctTree.h"
#include "SoundSystem.h"
#include "AnimationSystem.h"
#include "modelmatrixsystem.h"
#include "RenderSystem.h"
#include "PlayerSystem.h"
#include "FireSystem.h"
#include "BoxIconSystem.h"
#include "CollisionSystem.h"
#include "BoxIconSystem.h"
#include "AIsystem.h"
#include "HealthDisplaySystem.h"
#include "Trackingboxsystem.h"
#include "TargetingSystem.h"
#include "PhysicsSystem.h"
#include "lifetimesystem.h"
#include "HealthCheckSystem.h"
#include "TargetingSystem.h"
#include "CollisionResolveSystem.h"

MainGame::MainGame(std::shared_ptr<SceneManager> Sman, std::shared_ptr<entitymanager> myman, std::shared_ptr<EventSystem> ES) : scene(Sman,ES)
{
	myworld = std::make_shared<world>(myman);
	SDL_ShowCursor(SDL_DISABLE);
	wantedFPS = 1.0f / 60.0f;
}

void MainGame::init()
{
	myworld->setmaincam(std::make_shared<maincamera>());
	myworld->setoct(new OctTree(glm::vec3(-4000, -4000, -4000), glm::vec3(4000, 4000, 4000)));
	myworld->returnoct()->construct(NULL, 2);

	myworld->returnmanager()->create_skybox();
	enemycapship = myworld->returnmanager()->createEnemyCapital(glm::vec3(-500, 0, 100));
	allycapship = myworld->returnmanager()->createAllyCapital(glm::vec3(500, 0, 0));
	myworld->returnmanager()->createplayer(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyFighter(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyFighter(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyFighter(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyBomber(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyBomber(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createAllyBomber(myspawnsystem.getallyspawn());
	myworld->returnmanager()->createEnemyFighter(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyFighter(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyFighter(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyFighter(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyBomber(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyBomber(myspawnsystem.getenemyspawn());
	myworld->returnmanager()->createEnemyBomber(myspawnsystem.getenemyspawn());
	std::shared_ptr<dirlight> themainlight = std::make_shared<dirlight>();
	themainlight->setdir(glm::vec3(0.0, -1.0, 0.0));
	themainlight->setamb(glm::vec3(0.5, 0.5, 0.5));
	themainlight->setlightcolour(glm::vec3(1.0, 1.0, 1.0));
	myworld->returnmanager()->create_directionlight(themainlight);

	myRenderSystem = std::make_shared<RenderSystem>(myworld, EventManager);
	myupdatesystems.push_back(std::make_shared<SoundSystem>(myworld,0.2f, EventManager));
	myupdatesystems.push_back(std::make_shared<PlayerSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<AIsystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<FireSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<PhysicsSystsem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<BoxIconSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<Trackingboxsystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<HealthDisplaySystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<TargetingSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<CollisionSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<CollisionResolveSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<AnimationSystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<modelmatrixsystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<lifetimesystem>(myworld, EventManager));
	myupdatesystems.push_back(std::make_shared<HealthCheckSystem>(myworld, EventManager));
	

	updateEnts();
	myworld->getmaincam()->setProjection(glm::perspective(glm::radians(52.0f), (float)1280.0f / (float)720.0f, 0.1f, 10000.0f));

	std::shared_ptr<soundComponet> newsound = std::make_shared<soundComponet>();
	newsound->mysound = Resourecmanager::instance().getsound("MainMusic");
	newsound->mytype = newsound->repeate;
	newsound->position = glm::vec3(0, 0, 0);
	myworld->returnmanager()->createsound(newsound);

}

void MainGame::update(float dt, bool & go)
{
	myspawnsystem.createents(myworld->returnmanager());

	float tempdt = dt;
	bool yes = true;
	updateEnts();

	while (tempdt > 0.0)
	{
		float deltatime = std::fmin(tempdt, wantedFPS);
		for (int i = 0; i < myupdatesystems.size(); i++)
		{
			myupdatesystems[i]->update(tempdt, yes);
		}
		tempdt -= deltatime;
	}
	
	myspawnsystem.update(dt);

	endcondition(go);
}

void MainGame::draw(float dt)
{
	bool yes = true;

	myRenderSystem->update(dt, yes); // render our scene 

	if (allycapship != NULL && enemycapship != NULL)
	{
		int allyper = (allycapship->health / allycapship->maxhp) * 100;
		int enemyper = (enemycapship->health / enemycapship->maxhp) * 100;

		TextManager::instance().rendertext(std::to_string(enemyper) + "%", glm::vec3(1.0f, 1.0f, 1.0f), 1155.0f, 50.0f, 0.4f);
		TextManager::instance().rendertext(std::to_string(allyper) + "%", glm::vec3(1.0, 1.0, 1.0), 85.0f, 50.0f, 0.4f);
	}

	myspawnsystem.addrespawndata(myworld->returnmanager()->returntodelete());

	destroyEnts();
	myworld->returnmanager()->cleartodelete();
}

void MainGame::exit()
{
	myworld->returnoct()->cleanup();
	delete myworld->returnoct();
	myworld->exit();
	myworld.reset();

	for (int i = 0; i < myupdatesystems.size(); i++)
	{
		myupdatesystems.erase(myupdatesystems.begin() + i);
		i--;
	}


}

void MainGame::endcondition(bool &go)
{
	if (allycapship != NULL)
	{
		if (allycapship->health <= 0)
		{
			allycapship = NULL;
			
		}
		else if (enemycapship->health <= 0)
		{
			enemycapship = NULL;
		}
	}

}
