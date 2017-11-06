#include "entitymanager.h"

//we assign components to our needed entities 

entitymanager::entitymanager()
{
	
}

void entitymanager::delete_entity()
{
	entities.clear();
	tooadd.clear();
	todelete.clear();
}

void entitymanager::create_skybox() //create a skybox
{
	std::shared_ptr<ACC::entity> skyboxcomp;
	skyboxcomp = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Skybox);
	std::shared_ptr<transposecomponent> skytrans;
	skytrans = std::make_shared<transposecomponent>();
	skytrans->position = glm::vec3(0,0,0);
	skytrans->scale = glm::vec3(1, 1, 1);
	skyboxcomp->addcomponent(skytrans);

	std::shared_ptr<modelcomponent> skymod;
	skymod = std::make_shared<modelcomponent>();
	skymod->myshader = Resourecmanager::instance().getfromshadholder("skyboxshader");
	skymod->mymodel = Resourecmanager::instance().getfrommodelholder("skybox");
	skymod->drawme = true;
	skymod->depth = true;
	skymod->myshine = 1.0;
	skymod->mystrenght = 1.0;
	skymod->effectedbylights = false;
	skyboxcomp->addcomponent(skymod);

	entities[skyboxcomp->returnID()] = skyboxcomp;
	tooadd.push_back(skyboxcomp);
}

void entitymanager::create_directionlight(std::shared_ptr<dirlight> mydirinfo) //create a directional light
{
	std::shared_ptr<ACC::entity> dirlight = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Dirlight);

	std::shared_ptr<lightcomp> entlight = std::make_shared<lightcomp>();
	
	entlight->mylight = mydirinfo;
	dirlight->addcomponent(entlight);

	entities[dirlight->returnID()] = dirlight;
	tooadd.push_back(dirlight);

}

void entitymanager::createflashlight(std::shared_ptr<ACC::entity> parent) //create a flashlight
{ 
	std::shared_ptr<ACC::entity> spotl = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Spotlight);

	std::shared_ptr<spotlight> myspotlight = std::make_shared<spotlight>();
	myspotlight->setamb(glm::vec3(1.0, 0, 0));
	myspotlight->setlightcolour(glm::vec3(1.0f,0.0,0.0));
	myspotlight->setconstant(1.0f);
	myspotlight->setdir(glm::vec3(0, 0, 1));
	myspotlight->setlinear(0.045f);
	myspotlight->setquadratic(0.0075f);
	myspotlight->setpos(glm::vec3(0,0,1));
	myspotlight->setrad(glm::cos(glm::radians(16.5f)));
	myspotlight->settouterrad(glm::cos(glm::radians(60.5f)));

	std::shared_ptr<animationholdercomp> imapart = std::make_shared<animationholdercomp>();
	imapart->myowner = parent;
	imapart->offset = glm::vec3(0, 1, 0);
	imapart->mytype = imapart->light;
	spotl->addcomponent(imapart);


	std::shared_ptr<lightcomp> entlight = std::make_shared<lightcomp>();
	entlight->mylight = myspotlight;
	spotl->addcomponent(entlight);

	entities[spotl->returnID()] = spotl;
	tooadd.push_back(spotl);
}

void entitymanager::createplayer(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> player = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Player);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyFighter");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	player->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	player->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	player->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 100;
	health->maxhp = 100;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	player->addcomponent(health);

	std::shared_ptr<playercomp> playercom = std::make_shared<playercomp>();
	player->addcomponent(playercom);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 1;
	physicscomp->maxspeed = 100;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = false;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	player->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = false;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyFighter"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyFighter"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("allyfighterobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	player->addcomponent(collision);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 10;
	damagedeal->maxdamage = 50;
	player->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = true;
	teamcomp->Team = "ALLY";
	player->addcomponent(teamcomp);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 10;
	guns->guns.push_back(glm::vec3(1.0, 0.2, 8));
	guns->guns.push_back(glm::vec3(-1.0, 0.2, 8));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->currettime = 0.5;
	guns->firetime = 0.5;
	guns->speed = 300;
	guns->Wtype = guns->bullet;
	guns->firesound = Resourecmanager::instance().getsound("lazorsound");
	player->addcomponent(guns);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	player->addcomponent(iown);

	std::shared_ptr<mytarget> MT = std::make_shared<mytarget>();
	MT->target = NULL;
	player->addcomponent(MT);

	entities[player->returnID()] = player;
	tooadd.push_back(player);

	createcrosshair(player);
	healthbar(player, glm::vec3(7, 0.5, 0.5), glm::vec3(0, -15, 40), true);
	createdirarrow(player);
	createcapicon(glm::vec3(1, 1, 1), true, player);
	createcapicon(glm::vec3(1, 1, 1), false, player);

}

void entitymanager::createEnemyFighter(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> enemyfighter = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Fighter);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyFighter");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	enemyfighter->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	enemyfighter->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	enemyfighter->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 100;
	health->maxhp = 100;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	enemyfighter->addcomponent(health);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 1;
	physicscomp->maxspeed = 100.0f;
	physicscomp->maxforce = 60.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = true;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	enemyfighter->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = false;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("EnemyFighter"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("EnemyFighter"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("enemyfighterobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	enemyfighter->addcomponent(collision);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 10;
	damagedeal->maxdamage = 50;
	enemyfighter->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = false;
	teamcomp->Team = "ENEMY";
	enemyfighter->addcomponent(teamcomp);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 10;
	guns->Wtype = guns->bullet;
	guns->guns.push_back(glm::vec3(0.5, 0.2, 6));
	guns->guns.push_back(glm::vec3(-0.5, 0.2, 6));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->speed = 300;
	guns->currettime = 0.5;
	guns->firetime = 0.5;
	guns->firesound = Resourecmanager::instance().getsound("lazorsound");
	enemyfighter->addcomponent(guns);

	std::shared_ptr<manuvervector> man = std::make_shared<manuvervector>();
	enemyfighter->addcomponent(man);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	enemyfighter->addcomponent(target);
	
	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(enemyfighter);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	enemyfighter->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	enemyfighter->addcomponent(iown);

	entities[enemyfighter->returnID()] = enemyfighter;
	tooadd.push_back(enemyfighter);

	createUIbox(enemyfighter,7, false);
	trackerboxent(enemyfighter, false);
	healthbar(enemyfighter, glm::vec3(4, 0.5, 1), glm::vec3(0, 4, 0), false);
		
}

void entitymanager::createEnemyBomber(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> enemeybomber = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Bomber);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyBomber");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	enemeybomber->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	enemeybomber->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	enemeybomber->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 150;
	health->maxhp = 150;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	enemeybomber->addcomponent(health);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 1;
	physicscomp->maxspeed = 80.0f;
	physicscomp->maxforce = 40.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = true;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	enemeybomber->addcomponent(physicscomp);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	enemeybomber->addcomponent(target);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = false;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("EnemyBomber"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("EnemyBomber"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("enemybomberobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	enemeybomber->addcomponent(collision);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 10;
	damagedeal->maxdamage = 50;
	enemeybomber->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = false;
	teamcomp->Team = "ENEMY";
	enemeybomber->addcomponent(teamcomp);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 50;
	guns->Wtype = guns->bomb;
	guns->guns.push_back(glm::vec3(0.0, 0.0, 6));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->speed = 280;
	guns->currettime = 1,0;
	guns->firetime = 1.0;
	guns->firesound = Resourecmanager::instance().getsound("bombsound");
	enemeybomber->addcomponent(guns);

	std::shared_ptr<manuvervector> man = std::make_shared<manuvervector>();
	enemeybomber->addcomponent(man);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(enemeybomber);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	enemeybomber->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	enemeybomber->addcomponent(iown);

	entities[enemeybomber->returnID()] = enemeybomber;
	tooadd.push_back(enemeybomber);

	createUIbox(enemeybomber,7, false);
	trackerboxent(enemeybomber, false);
	healthbar(enemeybomber, glm::vec3(4, 0.5, 1), glm::vec3(0, 4, 0), false);

}

std::shared_ptr<healthcomponent> entitymanager::createEnemyCapital(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> EnemyCapital = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Capital);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyCapital");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	EnemyCapital->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	EnemyCapital->addcomponent(trans);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 200;
	physicscomp->maxspeed = 100.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = false;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	EnemyCapital->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = true;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("EnemyCapital"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("EnemyCapital"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("enemycapitalobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	EnemyCapital->addcomponent(collision);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	EnemyCapital->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 10000;
	health->maxhp = 10000;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	EnemyCapital->addcomponent(health);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = false;
	teamcomp->Team = "ENEMY";
	EnemyCapital->addcomponent(teamcomp);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 500;
	damagedeal->maxdamage = 500;
	EnemyCapital->addcomponent(damagedeal);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	EnemyCapital->addcomponent(target);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	AI->MovCon.setme(EnemyCapital);
	EnemyCapital->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	EnemyCapital->addcomponent(iown);

	entities[EnemyCapital->returnID()] = EnemyCapital;
	tooadd.push_back(EnemyCapital);

	createUIbox(EnemyCapital, 20, false);
	trackerboxent(EnemyCapital, false);
	healthbar(EnemyCapital, glm::vec3(15, 0.5, 1), glm::vec3(0, 10, 0), false);

	createCapTurret(EnemyCapital, false, glm::vec3(5.083, 3.342, -11.770));
	createCapTurret(EnemyCapital, false, glm::vec3(5.083,3.342,-0.523));
	createCapTurret(EnemyCapital, false, glm::vec3(5.083, 3.342, 9.472));

	createCapTurret(EnemyCapital, false, glm::vec3(-5.083, 3.342, -11.770));
	createCapTurret(EnemyCapital, false, glm::vec3(-5.083, 3.342, -0.523));
	createCapTurret(EnemyCapital, false, glm::vec3(-5.083, 3.342, 9.472));

	return health;

}

void entitymanager::createAllyFighter(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> AllyFighter = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Fighter);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyFighter");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	AllyFighter->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	AllyFighter->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	AllyFighter->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 100;
	health->maxhp = 100;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	AllyFighter->addcomponent(health);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 1;
	physicscomp->maxforce = 60.0f;
	physicscomp->maxspeed = 100.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = true;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	AllyFighter->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = false;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyFighter"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyFighter"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("allyfighterobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	AllyFighter->addcomponent(collision);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 10;
	damagedeal->maxdamage = 50;
	AllyFighter->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = true;
	teamcomp->Team = "ALLY";
	AllyFighter->addcomponent(teamcomp);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 10;
	guns->guns.push_back(glm::vec3(1.0, 0.2, 8));
	guns->guns.push_back(glm::vec3(-1.0, 0.2, 8));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->currettime = 0.5;
	guns->speed = 300;
	guns->firetime = 0.5;
	guns->Wtype = guns->bullet;
	guns->firesound = Resourecmanager::instance().getsound("lazorsound");
	AllyFighter->addcomponent(guns);

	std::shared_ptr<manuvervector> man = std::make_shared<manuvervector>();
	AllyFighter->addcomponent(man);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	AllyFighter->addcomponent(target);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(AllyFighter);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	AllyFighter->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	AllyFighter->addcomponent(iown);

	entities[AllyFighter->returnID()] = AllyFighter;
	tooadd.push_back(AllyFighter);

	createUIbox(AllyFighter, 7, true);
	trackerboxent(AllyFighter, true);
	healthbar(AllyFighter, glm::vec3(4, 0.5, 1), glm::vec3(0, 4, 0), false);

}

void entitymanager::createAllyBomber(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> allybomber = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Bomber);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyBomber");
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	allybomber->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	allybomber->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	allybomber->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 150;
	health->maxhp = 150;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	allybomber->addcomponent(health);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 1;
	physicscomp->speed = 0.0f;
	physicscomp->maxforce = 40.0f;
	physicscomp->maxspeed = 80.0f;
	physicscomp->physicsobject = true;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	allybomber->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = false;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyBomber"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyBomber"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("allybomberobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	allybomber->addcomponent(collision);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 10;
	damagedeal->maxdamage = 50;
	allybomber->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = true;
	teamcomp->Team = "ALLY";
	allybomber->addcomponent(teamcomp);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 50;
	guns->Wtype = guns->bomb;
	guns->guns.push_back(glm::vec3(0.0, 0.0, 8));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->speed = 280;
	guns->currettime = 1, 0;
	guns->firetime = 1.0;
	guns->firesound = Resourecmanager::instance().getsound("bombsound");
	allybomber->addcomponent(guns);

	std::shared_ptr<manuvervector> man = std::make_shared<manuvervector>();
	allybomber->addcomponent(man);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	allybomber->addcomponent(target);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(allybomber);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	allybomber->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	allybomber->addcomponent(iown);

	entities[allybomber->returnID()] = allybomber;
	tooadd.push_back(allybomber);

	createUIbox(allybomber, 7, true);
	trackerboxent(allybomber, true);
	healthbar(allybomber, glm::vec3(4, 0.5, 1),glm::vec3(0,4,0),false);

}

std::shared_ptr<healthcomponent> entitymanager::createAllyCapital(glm::vec3 pos)
{
	std::shared_ptr<ACC::entity> AllyCapital = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Capital);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyCapital");
	model->drawme = true;
	model->depth = true;
	model->myshine = 16.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	AllyCapital->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = pos;
	trans->scale = glm::vec3(1, 1, 1);
	AllyCapital->addcomponent(trans);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 200;
	physicscomp->maxspeed = 100.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = false;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	AllyCapital->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = true;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyCapital"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyCapital"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("allycapitalshipobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(pos, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, pos);
	AllyCapital->addcomponent(collision);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	AllyCapital->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 10000;
	health->maxhp = 10000;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	AllyCapital->addcomponent(health);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = true;
	teamcomp->Team = "ALLY";
	AllyCapital->addcomponent(teamcomp);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	AllyCapital->addcomponent(target);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 500;
	damagedeal->maxdamage = 500;
	AllyCapital->addcomponent(damagedeal);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(AllyCapital);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	AllyCapital->addcomponent(AI);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	AllyCapital->addcomponent(iown);

	entities[AllyCapital->returnID()] = AllyCapital;
	tooadd.push_back(AllyCapital);

	createUIbox(AllyCapital, 20, true);
	trackerboxent(AllyCapital, true);
	healthbar(AllyCapital, glm::vec3(15, 0.5, 1), glm::vec3(0, 10, 0), false);

    createCapTurret(AllyCapital, true, glm::vec3(7.217, 3.2, 4.978));
	createCapTurret(AllyCapital, true, glm::vec3(8.031, 3.2, 10.738));
	createCapTurret(AllyCapital, true, glm::vec3(9.151, 3.2, 17.051));

	createCapTurret(AllyCapital, true, glm::vec3(-3.371, 3.2, 4.978));
	createCapTurret(AllyCapital, true, glm::vec3(-3.948, 3.2, 11.390));
	createCapTurret(AllyCapital, true, glm::vec3(-4.959, 3.2, 17.051));
		
	return health;
}
void entitymanager::createCapTurret(std::shared_ptr<ACC::entity> mycap, bool team, glm::vec3 offset)
{
	std::shared_ptr<transposecomponent> captrans = mycap->getcomponent<transposecomponent>();

	std::shared_ptr<ACC::entity> turretEntity = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Turrent);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	if (team)
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyTurret");
	}
	else
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyTurret");
	}
	model->drawme = true;
	model->depth = true;
	model->myshine = 16.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	turretEntity->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = captrans->position + offset;
	trans->scale = glm::vec3(1, 1, 1);
	turretEntity->addcomponent(trans);

	std::shared_ptr<animationholdercomp> anim = std::make_shared<animationholdercomp>();
	anim->mytype = anim->trans;
	anim->myowner = mycap;
	anim->offset = offset;
	turretEntity->addcomponent(anim);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	turretEntity->addcomponent(dir);

	std::shared_ptr<healthcomponent> health = std::make_shared<healthcomponent>();
	health->health = 500;
	health->maxhp = 500;
	health->hitdelete.first = false;
	health->hitdelete.second = false;
	health->deathsound.first = Resourecmanager::instance().getsound("explosion");
	health->deathsound.second = true;
	turretEntity->addcomponent(health);

	std::shared_ptr<physics> physicscomp = std::make_shared<physics>();
	physicscomp->mass = 50;
	physicscomp->maxspeed = 0.0f;
	physicscomp->speed = 0.0f;
	physicscomp->physicsobject = false;
	physicscomp->velocity = glm::vec3(0, 0, 0);
	turretEntity->addcomponent(physicscomp);

	std::shared_ptr<collisioncomp> collision = std::make_shared<collisioncomp>();
	collision->imstatic = true;
	collision->physicseffected = true;
	collision->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyTurret"));
	collision->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyTurret"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("turretobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		collision->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		collision->myobb[i]->update(trans->position, trans->myquat);
	}
	collision->myAABB->update(trans->myquat, trans->position);
	collision->dontcolwith.push_back(mycap->returnID());
	turretEntity->addcomponent(collision);

	std::shared_ptr<turretcomp> turr = std::make_shared<turretcomp>();
	if (offset.x > 0.0)
	{
		turr->basedir = glm::vec3(1, 0, 0);
	}
	else
	{
		turr->basedir = glm::vec3(-1, 0, 0);
	}
	turretEntity->addcomponent(turr);

	std::shared_ptr<damagedelt> damagedeal = std::make_shared<damagedelt>();
	damagedeal->damage = 70;
	damagedeal->maxdamage = 70;
	turretEntity->addcomponent(damagedeal);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = team;
	if (team)
	{
		teamcomp->Team = "ALLY";
	}
	else
	{
		teamcomp->Team = "ENEMY";
	}
	turretEntity->addcomponent(teamcomp);

	std::shared_ptr<targetable> target = std::make_shared<targetable>();
	turretEntity->addcomponent(target);

	std::shared_ptr<thingsiown> iown = std::make_shared<thingsiown>();
	turretEntity->addcomponent(iown);

	std::shared_ptr<AIcomp> AI = std::make_shared<AIcomp>();
	AI->free = true;
	AI->MovCon.setme(turretEntity);
	AI->MovCon.setsepsphere(collision->mysphere->rad * 1.2);
	turretEntity->addcomponent(AI);

	entities[turretEntity->returnID()] = turretEntity;
	tooadd.push_back(turretEntity);

	createTurretGuns(turretEntity, team);

	createUIbox(turretEntity, 5, team);
	trackerboxent(turretEntity, team);
	healthbar(turretEntity, glm::vec3(3, 0.5, 1), glm::vec3(0, 3, 0), false);

}


void entitymanager::createTurretGuns(std::shared_ptr<ACC::entity> myturret, bool team)
{
	std::shared_ptr<transposecomponent> turrettrans = myturret->getcomponent<transposecomponent>();

	std::shared_ptr<ACC::entity> turretguns = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Turretguns);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lightshader");
	if (team)
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyTurretGuns");
	}
	else
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyTurretGuns");
	}
	model->drawme = true;
	model->depth = true;
	model->myshine = 16.0;
	model->mystrenght = 1.0;
	model->effectedbylights = true;
	turretguns->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = glm::vec3(-0.247, 0, 1.395) + turrettrans->position;
	trans->scale = glm::vec3(1, 1, 1);
	turretguns->addcomponent(trans);

	std::shared_ptr<animationholdercomp> anim = std::make_shared<animationholdercomp>();
	anim->mytype = anim->trans;
	anim->myowner = myturret;
	anim->offset = glm::vec3(-0.247, 0, 1.395);
	turretguns->addcomponent(anim);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = trans->myquat * dir->direction;
	turretguns->addcomponent(dir);

	std::shared_ptr<fighterguns> guns = std::make_shared<fighterguns>();
	guns->damage = 30;
	guns->guns.push_back(glm::vec3(1.0, 0.0, 3));
	guns->guns.push_back(glm::vec3(-1.0, 0.0, 3));
	guns->numberofguns = guns->guns.size();
	guns->fire = false;
	guns->speed = 250;
	guns->currettime = 2;
	guns->firetime = 2;
	guns->Wtype = guns->bullet;
	guns->firesound = Resourecmanager::instance().getsound("lazorsound");
	turretguns->addcomponent(guns);

	std::shared_ptr<teamcomponent> teamcomp = std::make_shared<teamcomponent>();
	teamcomp->ally = team;
	if (team)
	{
		teamcomp->Team = "ALLY";
	}
	else
	{
		teamcomp->Team = "ENEMY";
	}
	turretguns->addcomponent(teamcomp);

	std::shared_ptr<thingsiown> iown = myturret->getcomponent<thingsiown>();
	iown->stuffiown.push_back(turretguns);


	entities[turretguns->returnID()] = turretguns;
	tooadd.push_back(turretguns);

}


void entitymanager::createcrosshair(std::shared_ptr<ACC::entity> player)
{
	std::shared_ptr<ACC::entity> crosshair = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Crosshair);
	
	std::shared_ptr<animationholdercomp> anim = std::make_shared<animationholdercomp>();
	anim->mytype = anim->trans;
	anim->myowner = player;
	anim->offset = glm::vec3(0, 0, 10.0f);
	crosshair->addcomponent(anim);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("lowalpha");
	model->mymodel = Resourecmanager::instance().getfrommodelholder("Crosshair");
	model->drawme = true;
	model->depth = false;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = false;
	crosshair->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->scale = glm::vec3(1,1,1);
	trans->position = anim->offset;
	crosshair->addcomponent(trans);

	std::shared_ptr<thingsiown> iown = player->getcomponent<thingsiown>();
	iown->stuffiown.push_back(crosshair);

	entities[crosshair->returnID()] = crosshair;
	tooadd.push_back(crosshair);

}

void entitymanager::createbullet(projectileinfo info)
{
	std::shared_ptr<ACC::entity> bullet = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Bullet);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	std::shared_ptr<teamcomponent> team = std::make_shared<teamcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("textureshader");
	if (info.team == "ALLY")
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyBullet");
		team->ally = true;
	}
	else
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyBullet");
		team->ally = false;
	}
	team->Team = info.team;
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = false;
	bullet->addcomponent(model);
	bullet->addcomponent(team);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = info.pos;
	trans->myquat;
	trans->scale = glm::vec3(1, 1, 1);
	bullet->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = info.dir;
	bullet->addcomponent(dir);

	std::shared_ptr<lifespan> life = std::make_shared<lifespan>();
	life->lifetime = 10;
	bullet->addcomponent(life);

	std::shared_ptr<damagedelt> damage = std::make_shared<damagedelt>();
	damage->damage = damage->maxdamage = info.dam;
	damage->damagesound = Resourecmanager::instance().getsound("Bulletcol");
	bullet->addcomponent(damage);

	std::shared_ptr<healthcomponent> hp = std::make_shared<healthcomponent>();
	hp->health = hp->maxhp = 1;
	hp->hitdelete.first = true;
	hp->hitdelete.second = false;
	bullet->addcomponent(hp);

	std::shared_ptr<physics> speedcomp = std::make_shared<physics>();
	speedcomp->mass = 1;
	speedcomp->physicsobject = false;
	speedcomp->speed = info.speed;
	speedcomp->maxspeed = 300;
	speedcomp->velocity = dir->newdirect * speedcomp->maxspeed;
	bullet->addcomponent(speedcomp);

	std::shared_ptr<collisioncomp> col = std::make_shared<collisioncomp>();
	col->physicseffected = false;
	col->imstatic = false;
	col->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyBullet"));
	col->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyBullet"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("bulletobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		col->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		col->myobb[i]->update(info.pos, trans->myquat);
	}
	col->myAABB->update(trans->myquat, info.pos);
	bullet->addcomponent(col);

	entities[bullet->returnID()] = bullet;
	tooadd.push_back(bullet);
}

void entitymanager::createbomb(projectileinfo info)
{
	std::shared_ptr<ACC::entity> bomb = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Bomb);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	std::shared_ptr<teamcomponent> team = std::make_shared<teamcomponent>();
	model = std::make_shared<modelcomponent>();
	model->myshader = Resourecmanager::instance().getfromshadholder("textureshader");
	if (info.team == "ALLY")
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("AllyBomb");
		team->ally = true;
	}
	else
	{
		model->mymodel = Resourecmanager::instance().getfrommodelholder("EnemyBomb");
		team->ally = false;
	}
	team->Team = info.team;
	model->drawme = true;
	model->depth = true;
	model->myshine = 1.0;
	model->mystrenght = 1.0;
	model->effectedbylights = false;
	bomb->addcomponent(model);
	bomb->addcomponent(team);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->position = info.pos;
	trans->myquat;
	trans->scale = glm::vec3(1, 1, 1);
	bomb->addcomponent(trans);

	std::shared_ptr<directioncomponent> dir = std::make_shared<directioncomponent>();
	dir->direction = glm::vec3(0, 0, 1);
	dir->newdirect = info.dir;
	bomb->addcomponent(dir);

	std::shared_ptr<lifespan> life = std::make_shared<lifespan>();
	life->lifetime = 10;
	bomb->addcomponent(life);

	std::shared_ptr<damagedelt> damage = std::make_shared<damagedelt>();
	damage->damage = damage->maxdamage = info.dam;
	damage->damagesound = Resourecmanager::instance().getsound("Bulletcol");
	bomb->addcomponent(damage);

	std::shared_ptr<healthcomponent> hp = std::make_shared<healthcomponent>();
	hp->health = hp->maxhp = 1;
	hp->hitdelete.first = true;
	hp->hitdelete.second = false;
	bomb->addcomponent(hp);

	std::shared_ptr<physics> speedcomp = std::make_shared<physics>();
	speedcomp->mass = 1;
	speedcomp->physicsobject = false;
	speedcomp->speed = info.speed;
	speedcomp->maxspeed = 280;
	speedcomp->velocity = dir->newdirect * speedcomp->maxspeed;
	bomb->addcomponent(speedcomp);

	std::shared_ptr<collisioncomp> col = std::make_shared<collisioncomp>();
	col->physicseffected = false;
	col->imstatic = false;
	col->myAABB = std::make_shared<AABB>(Resourecmanager::instance().getAABB("AllyBomb"));
	col->mysphere = std::make_shared<sphere>(Resourecmanager::instance().getsphere("AllyBomb"));
	std::vector<OBB> myobbs = Resourecmanager::instance().getOBB("bombobb");
	for (int i = 0; i < myobbs.size(); i++)
	{
		col->myobb.push_back(std::make_shared<OBB>(myobbs[i]));
		col->myobb[i]->update(info.pos, trans->myquat);
	}
	col->myAABB->update(trans->myquat, info.pos);
	bomb->addcomponent(col);

	entities[bomb->returnID()] = bomb;
	tooadd.push_back(bomb);


}

void entitymanager::createUIbox(std::shared_ptr<ACC::entity> myent, int maxscale, bool team)
{
	std::shared_ptr<ACC::entity> UIbox = std::make_shared<ACC::entity>(IDs.getID(), enttypes::UIBox);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model->depth = false;
	model->drawme = true;
	model->effectedbylights = false;
	model->mymodel = Resourecmanager::instance().getfrommodelholder("UIbox");
	model->myshader = Resourecmanager::instance().getfromshadholder("teamalpha");
	uisettings start;
	start.setscale(glm::vec3(1, 1, 1));
	start.setteam(team);
	model->extra = std::make_unique<uisettings>(start);
	UIbox->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->scale = glm::vec3(1, 1, 1);
	UIbox->addcomponent(trans);

	std::shared_ptr<uibox> UIboxcomp = std::make_shared<uibox>();
	UIboxcomp->myship = myent;
	UIboxcomp->maxsizebox = maxscale;
	UIboxcomp->nowsize = maxscale;
	UIbox->addcomponent(UIboxcomp);

	std::shared_ptr<thingsiown> iown = myent->getcomponent<thingsiown>();
	iown->stuffiown.push_back(UIbox);


	entities[UIbox->returnID()] = UIbox;
	tooadd.push_back(UIbox);

}

void entitymanager::trackerboxent(std::shared_ptr<ACC::entity> myent, bool team)
{
	std::shared_ptr<ACC::entity> trackboxe = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Trackerbox);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model->depth = false;
	model->drawme = true;
	model->effectedbylights = false;
	model->mymodel = Resourecmanager::instance().getfrommodelholder("trackerbox");
	model->myshader = Resourecmanager::instance().getfromshadholder("teamalpha");
	uisettings start;
	start.setscale(glm::vec3(2, 2, 2));
	start.setteam(team);
	model->extra = std::make_unique<uisettings>(start);
	trackboxe->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->scale = glm::vec3(2,2,2);
	trackboxe->addcomponent(trans);

	std::shared_ptr<trackerbox> trackbox = std::make_shared<trackerbox>();
	trackbox->anglecutoff = 40;
	trackbox->unit = myent;
	trackboxe->addcomponent(trackbox);

	std::shared_ptr<thingsiown> iown = myent->getcomponent<thingsiown>();
	iown->stuffiown.push_back(trackboxe);

	entities[trackboxe->returnID()] = trackboxe;
	tooadd.push_back(trackboxe);

}

void entitymanager::createdirarrow(std::shared_ptr<ACC::entity> myent)
{
	std::shared_ptr<transposecomponent> playertrans = myent->getcomponent<transposecomponent>();

	std::shared_ptr<ACC::entity> trackerarrows = std::make_shared<ACC::entity>(IDs.getID(), enttypes::TrackerArrows);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model->depth = false;
	model->drawme = false;
	model->effectedbylights = false;
	model->mymodel = Resourecmanager::instance().getfrommodelholder("targetarrows");
	model->myshader = Resourecmanager::instance().getfromshadholder("lowalpha");
	trackerarrows->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->scale = glm::vec3(1, 1, 1);
	trans->position = playertrans->position + glm::vec3(0, 0, 40);
	trackerarrows->addcomponent(trans);

	std::shared_ptr<trackingarrowscomp> arrowcomp = std::make_shared<trackingarrowscomp>();
	arrowcomp->targetbox = NULL;
	arrowcomp->offset = glm::vec3(0, 0, 40);
	arrowcomp->player = myent;
	trackerarrows->addcomponent(arrowcomp);

	std::shared_ptr<thingsiown> iown = myent->getcomponent<thingsiown>();
	iown->stuffiown.push_back(trackerarrows);

	entities[trackerarrows->returnID()] = trackerarrows;
	tooadd.push_back(trackerarrows);

}

void entitymanager::healthbar(std::shared_ptr<ACC::entity> myent, glm::vec3 size, glm::vec3 offset, bool playerhealth)
{
	std::shared_ptr<ACC::entity> healthbar = std::make_shared<ACC::entity>(IDs.getID(), enttypes::HealthBar);

	std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
	model->depth = false;
	model->drawme = true;
	model->effectedbylights = false;
	model->mymodel = Resourecmanager::instance().getfrommodelholder("HealthBar");
	model->myshader = Resourecmanager::instance().getfromshadholder("healthshader");
	healthsettings start;
	start.setscale(size);
	start.setnormhealth(1.0f);
	model->extra = std::make_unique<healthsettings>(start);
	healthbar->addcomponent(model);

	std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
	trans->scale = size;
	healthbar->addcomponent(trans);

	std::shared_ptr<unithealthbar> healthbarcomp = std::make_shared<unithealthbar>();
	healthbarcomp->unit = myent;
	healthbarcomp->Doffset = offset;
	healthbarcomp->playerhealth = playerhealth;
	healthbar->addcomponent(healthbarcomp);


	std::shared_ptr<thingsiown> iown = myent->getcomponent<thingsiown>();
	iown->stuffiown.push_back(healthbar);

	entities[healthbar->returnID()] = healthbar;
	tooadd.push_back(healthbar);

}

void entitymanager::createcapicon(glm::vec3 maxscale, bool ally, std::shared_ptr<ACC::entity> player)
{
	if (ally == true)
	{
		std::shared_ptr<ACC::entity> allycapicon = std::make_shared<ACC::entity>(IDs.getID(), enttypes::AllyCapIcon);

		std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
		model->depth = false;
		model->drawme = true;
		model->effectedbylights = false;
		model->mymodel = Resourecmanager::instance().getfrommodelholder("allycapicon");
		model->myshader = Resourecmanager::instance().getfromshadholder("lowalpha");
		allycapicon->addcomponent(model);

		std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
		trans->scale = glm::vec3(1, 1, 1);
		allycapicon->addcomponent(trans);

		std::shared_ptr<animationholdercomp> anim = std::make_shared<animationholdercomp>();
		anim->mytype = anim->trans;
		anim->myowner = player;
		anim->offset = glm::vec3(7.0f, -3.8f, 15.0f);
		allycapicon->addcomponent(anim);

		std::shared_ptr<thingsiown> iown = player->getcomponent<thingsiown>();
		iown->stuffiown.push_back(allycapicon);

		entities[allycapicon->returnID()] = allycapicon;
		tooadd.push_back(allycapicon);
	}
	else
	{
		std::shared_ptr<ACC::entity> enemycapicon = std::make_shared<ACC::entity>(IDs.getID(), enttypes::EnemyCapIcon);

		std::shared_ptr<modelcomponent> model = std::make_shared<modelcomponent>();
		model->depth = false;
		model->drawme = true;
		model->effectedbylights = false;
		model->mymodel = Resourecmanager::instance().getfrommodelholder("enemycapicon");
		model->myshader = Resourecmanager::instance().getfromshadholder("lowalpha");
		enemycapicon->addcomponent(model);

		std::shared_ptr<transposecomponent> trans = std::make_shared<transposecomponent>();
		trans->scale = maxscale;
		trans->position = glm::vec3(-7.0f, -3.8f, 15.0f);
		enemycapicon->addcomponent(trans);

		std::shared_ptr<animationholdercomp> anim = std::make_shared<animationholdercomp>();
		anim->mytype = anim->trans;
		anim->offset = glm::vec3(-7.0f, -3.8f, 15.0f);
		anim->myowner = player;
		enemycapicon->addcomponent(anim);

		std::shared_ptr<thingsiown> iown = player->getcomponent<thingsiown>();
		iown->stuffiown.push_back(enemycapicon);

		entities[enemycapicon->returnID()] = enemycapicon;
		tooadd.push_back(enemycapicon);

	}
	
}

void entitymanager::createsound(std::shared_ptr<soundComponet> soundcomp)
{
	std::shared_ptr<ACC::entity> sound = std::make_shared<ACC::entity>(IDs.getID(), enttypes::Sound);
	soundcomp->mysound.sourcesetup();
	soundcomp->mysound.setpos(soundcomp->position);
	sound->addcomponent(soundcomp);

	entities[sound->returnID()] = sound;
	tooadd.push_back(sound);
}


void entitymanager::cleartodelete() //delete all entities we no longer need 
{
	for (int i = 0; i < todelete.size(); i++)
	{
		std::unordered_map<unsigned long int, std::shared_ptr<ACC::entity>>::const_iterator got = entities.find(todelete[i]->returnID());
		if (got != entities.end())
		{
			entities[todelete[i]->returnID()]->setdeleteme();
		}
		entities.erase(todelete[i]->returnID());
		todelete.erase(todelete.begin() + i);
		i--;
	}
	todelete.clear();
}