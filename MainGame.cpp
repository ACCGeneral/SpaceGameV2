#include "MainGame.h"
#include "maincamera.h"
#include "OctTree.h"

MainGame::MainGame(std::shared_ptr<SceneManager> Sman, std::shared_ptr<entitymanager> myman) : scene(Sman)
{
	myworld = std::make_shared<world>(myman);
	SDL_ShowCursor(SDL_DISABLE);
	wantedFPS = 1.0f / 60.0f;
}

void MainGame::init()
{
	MyManager = std::make_shared<ACC::System_Manager>(getself(), myworld);
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

	std::shared_ptr<RenderSystem> myrendersystem = std::make_shared<RenderSystem>(myworld);
	MyManager->setrender(myrendersystem);
	std::shared_ptr<PlayerSystem> playersys = std::make_shared<PlayerSystem>(myworld);
	MyManager->addsystem(playersys);
	std::shared_ptr<AIsystem> aisys = std::make_shared<AIsystem>(myworld);
	MyManager->addsystem(aisys);
	std::shared_ptr<FireSystem> firesys = std::make_shared<FireSystem>(myworld);
	MyManager->addsystem(firesys);
	std::shared_ptr<PhysicsSystsem> physiscs = std::make_shared<PhysicsSystsem>(myworld);
	MyManager->addsystem(physiscs);
	std::shared_ptr<BoxIconSystem> boxsys = std::make_shared<BoxIconSystem>(myworld);
	MyManager->addsystem(boxsys);
	std::shared_ptr<Trackingboxsystem> trackboxsys = std::make_shared<Trackingboxsystem>(myworld);
	MyManager->addsystem(trackboxsys);
	std::shared_ptr<HealthDisplaySystem> healthsystem = std::make_shared<HealthDisplaySystem>(myworld);
	MyManager->addsystem(healthsystem);
	std::shared_ptr<TargetingSystem> trackerarrows = std::make_shared<TargetingSystem>(myworld);
	MyManager->addsystem(trackerarrows);
	std::shared_ptr<CollisionSystem> colsystem = std::make_shared<CollisionSystem>(myworld);
	MyManager->addsystem(colsystem);
	std::shared_ptr<AnimationSystem> anisystem = std::make_shared<AnimationSystem>(myworld);
	MyManager->addsystem(anisystem);
	std::shared_ptr<modelmatrixsystem> modelupdate = std::make_shared<modelmatrixsystem>(myworld);
	MyManager->addsystem(modelupdate);
	std::shared_ptr<lifetimesystem> lifesystem = std::make_shared<lifetimesystem>(myworld);
	MyManager->addsystem(lifesystem);
	std::shared_ptr<HealthCheckSystem> healthchecksystem = std::make_shared<HealthCheckSystem>(myworld);
	MyManager->addsystem(healthchecksystem);

	MyManager->updatesystements();
	myworld->getmaincam()->setProjection(glm::perspective(glm::radians(52.0f), (float)1280.0f / (float)720.0f, 0.1f, 10000.0f));

	mainsound = Resourecmanager::instance().getsound("MainMusic");
	mainsound.setpos(glm::vec3(0, 0, 0));
	mainsound.playsound();

}

void MainGame::update(float dt, bool & go)
{
	mainsound.repeatcheck();
	myspawnsystem.createents(myworld->returnmanager());

	float tempdt = dt;

	bool yes = true;
	MyManager->updatesystements(); //add the new entities created 


	while (tempdt > 0.0)
	{
		float deltatime = std::fmin(tempdt, wantedFPS);
		MyManager->update(deltatime, go); // updata all none render systems
		tempdt -= deltatime;
	}
	
	myspawnsystem.update(dt);

	if (allycapship != NULL)
	{
		if (allycapship->health <= 0 || enemycapship->health <= 0)
		{
			allycapship = NULL;
			enemycapship = NULL;

			return;
		}
	}
	
}

void MainGame::draw(float dt)
{
	bool yes = true;
	MyManager->render(dt, yes); // render our scene 

	if (allycapship != NULL && enemycapship != NULL)
	{
		int allyper = (allycapship->health / allycapship->maxhp) * 100;
		int enemyper = (enemycapship->health / enemycapship->maxhp) * 100;

		TextManager::instance().rendertext(std::to_string(enemyper) + "%", glm::vec3(1.0f, 1.0f, 1.0f), 1155.0f, 50.0f, 0.4f);
		TextManager::instance().rendertext(std::to_string(allyper) + "%", glm::vec3(1.0, 1.0, 1.0), 85.0f, 50.0f, 0.4f);
	}

	myspawnsystem.addrespawndata(myworld->returnmanager()->returntodelete());

	MyManager->deletesystements();
	myworld->returnmanager()->cleartodelete();
}

void MainGame::exit()
{
	myworld->returnoct()->cleanup();
	delete myworld->returnoct();
	myworld->exit();
	myworld.reset();
	MyManager->cleanupsystems();

}
