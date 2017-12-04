#include "Resourcemanager.h"

Resourecmanager & Resourecmanager::instance() //get instance of our resource manager 
{
	static Resourecmanager myinstance;

	return myinstance;
}

std::shared_ptr<shader> Resourecmanager::getfromshadholder(std::string name) //get a shader 
{
	return shadholder.getshader(name);
}

std::shared_ptr<model> Resourecmanager::getfrommodelholder(std::string name) //get a model 
{
	return modholder.getmode(name);
}

AABB Resourecmanager::getAABB(std::string name) //get the aabb data for a model
{
	return modholder.getAABB(name);
}

OBB Resourecmanager::getOBBfrommodel(std::string name)
{
	return modholder.GetOBB(name);
}

sphere Resourecmanager::getsphere(std::string name)
{
	return modholder.getsphere(name);
}

std::vector<OBB> Resourecmanager::getOBB(std::string name)
{
	return myOBBholder.getOBB(name);
}

sound Resourecmanager::getsound(std::string name)
{
	return mysoundholder.getsound(name);
}

void Resourecmanager::addshader(std::shared_ptr<shader> newshader, std::string name) //add a new shader to the shader holder 
{
	shadholder.addshader(newshader, name);

}

void Resourecmanager::addmodels(std::string modelpath, std::string modelname) //add a normal model
{
	modholder.addmodel(modelpath, modelname);
}

void Resourecmanager::addskybox(std::vector<std::string> cube, std::string modelpath, std::string modelname) //add a skybox model
{
	modholder.addskybox(modelpath, cube, modelname);
}

void Resourecmanager::abbobb(std::string filename, std::string name)
{
	myOBBholder.addOBB(filename, name);
}

void Resourecmanager::addsound(sound newsound, std::string name)
{
	mysoundholder.addsound(newsound, name);
}

void Resourecmanager::modelsinit() //create all of our models that we might need 
{
	addmodels("Models/AllyBomb/AllyBomb.obj","AllyBomb");
	addmodels("Models/AllyBomber/AllyBomber.obj", "AllyBomber");
	addmodels("Models/AllyBullet/AllyBullet.obj", "AllyBullet");
	addmodels("Models/AllyCapital/AllyCapital.obj", "AllyCapital");
	addmodels("Models/AllyFighter/AllyFighter.obj", "AllyFighter");
	addmodels("Models/AllyTurret/Turret.obj", "AllyTurret");
	addmodels("Models/AllyTurretguns/TurretGuns.obj", "AllyTurretGuns");

	addmodels("Models/EnemyBomb/EnemyBomb.obj", "EnemyBomb");
	addmodels("Models/EnemyBomber/enemybomber.obj", "EnemyBomber");
	addmodels("Models/EnemyBullet/EnemyBullet.obj", "EnemyBullet");
	addmodels("Models/EnemyFighter/EnemyFighter.obj", "EnemyFighter");
	addmodels("Models/EnemyCapital/EnemyCapital.obj", "EnemyCapital");
	addmodels("Models/EnemyTurret/Turret.obj", "EnemyTurret");
	addmodels("Models/EnemyTurretGuns/TurretGuns.obj", "EnemyTurretGuns");


	addmodels("Models/Crosshair/Crosshair.obj", "Crosshair");
	addmodels("Models/ObjectBox/UIbox.obj", "UIbox");
	addmodels("Models/targetarrows/targetarrows.obj", "targetarrows");
	addmodels("Models/trackerbox/trackerbox.obj", "trackerbox");
	addmodels("Models/HealthBar/HealthBar.obj", "HealthBar");

	addmodels("Models/AllyCapIcon/AllyShipIcon.obj", "allycapicon");
	addmodels("Models/EnemyCapIcon/EnemyCapIcon.obj", "enemycapicon");

	std::vector<std::string> cube;
	cube.push_back("skyboximages/right.bmp");
	cube.push_back("skyboximages/left.bmp");
	cube.push_back("skyboximages/up.bmp");
	cube.push_back("skyboximages/down.bmp");
	cube.push_back("skyboximages/back.bmp");
	cube.push_back("skyboximages/front.bmp");
	addskybox(cube, "Models/Skybox/skybox.obj","skybox");

}

void Resourecmanager::obbinit()
{
	myOBBholder.addOBB("obbdata//allyfighterobb.txt", "allyfighterobb");
	myOBBholder.addOBB("obbdata//allycapitalshipobb.txt", "allycapitalshipobb");
	myOBBholder.addOBB("obbdata//allybomberobb.txt", "allybomberobb");
	myOBBholder.addOBB("obbdata//bombobb.txt", "bombobb");
	myOBBholder.addOBB("obbdata//bulletobb.txt", "bulletobb");
	myOBBholder.addOBB("obbdata//enemybomberobb.txt", "enemybomberobb");
	myOBBholder.addOBB("obbdata//enemyfighterobb.txt", "enemyfighterobb");
	myOBBholder.addOBB("obbdata//enemycapitalobb.txt", "enemycapitalobb");
	myOBBholder.addOBB("obbdata//turretobb.txt", "turretobb");

}

void Resourecmanager::shaderinit() //create all of our shaders that we might need 
{
	std::shared_ptr<shader> skybox;
	skybox = std::make_shared<shader>();
	skybox->loadshader("shaders//skyboxvertex.txt", GL_VERTEX_SHADER);
	skybox->loadshader("shaders//skyboxfragment.txt", GL_FRAGMENT_SHADER);
	skybox->linkshader();
	skybox->linkcheck();
	addshader(skybox, "skyboxshader");

	std::shared_ptr<shader> justtexture;
	justtexture = std::make_shared<shader>();
	justtexture->loadshader("shaders//vertexshader.txt", GL_VERTEX_SHADER);
	justtexture->loadshader("shaders//fragmentshader.txt", GL_FRAGMENT_SHADER);
	justtexture->linkshader();
	justtexture->linkcheck();
	addshader(justtexture, "textureshader");

	std::shared_ptr<shader> alphashader;
	alphashader = std::make_shared<shader>();
	alphashader->loadshader("shaders//vertexshaderalpha.txt", GL_VERTEX_SHADER);
	alphashader->loadshader("shaders//fragmentshaderalpha.txt", GL_FRAGMENT_SHADER);
	alphashader->linkshader();
	alphashader->linkcheck();
	addshader(alphashader, "lowalpha");

	std::shared_ptr<shader> uiteambase;
	uiteambase = std::make_shared<shader>();
	uiteambase->loadshader("shaders//billboardVshader.txt", GL_VERTEX_SHADER);
	uiteambase->loadshader("shaders//fragmentshaderteam.txt", GL_FRAGMENT_SHADER);
	uiteambase->linkshader();
	uiteambase->linkcheck();
	addshader(uiteambase, "teamalpha");

	std::shared_ptr<shader> lightshader;
	lightshader = std::make_shared<shader>();
	lightshader->loadshader("shaders//vertexshadertexture_lights.txt", GL_VERTEX_SHADER);
	lightshader->loadshader("shaders//fragmentshadertexture_Lights.txt", GL_FRAGMENT_SHADER);
	lightshader->linkshader();
	lightshader->linkcheck();
	addshader(lightshader, "lightshader");

	std::shared_ptr<shader> textshader;
	textshader = std::make_shared<shader>();
	textshader->loadshader("shaders//TextVertex.txt", GL_VERTEX_SHADER);
	textshader->loadshader("shaders//TextFragment.txt", GL_FRAGMENT_SHADER);
	textshader->linkshader();
	textshader->linkcheck();
	addshader(textshader, "textshader");

	std::shared_ptr<shader> healthshader;
	healthshader = std::make_shared<shader>();
	healthshader->loadshader("shaders//billboardVshader.txt", GL_VERTEX_SHADER);
	healthshader->loadshader("shaders//fragmentshaderhealth.txt", GL_FRAGMENT_SHADER);
	healthshader->linkshader();
	healthshader->linkcheck();
	addshader(healthshader, "healthshader");

}

void Resourecmanager::soundsinit()
{
	sound lazorsound;
	lazorsound.setup("Sounds//LazorFire.ogg");
	mysoundholder.addsound(lazorsound, "lazorsound");

	sound bombsound;
	bombsound.setup("Sounds//BombFire.ogg");
	mysoundholder.addsound(bombsound, "bombsound");

	sound collisionsound;
	collisionsound.setup("Sounds//Collision.ogg");
	mysoundholder.addsound(collisionsound, "Collision");

	sound explosion;
	explosion.setup("Sounds//Explotion.ogg");
	mysoundholder.addsound(explosion, "explosion");

	sound bulletHit;
	bulletHit.setup("Sounds//BulletHit.ogg");
	mysoundholder.addsound(bulletHit, "Bulletcol");

	sound mainmusic;
	mainmusic.setup("Sounds//MainSong.ogg");
	mysoundholder.addsound(mainmusic, "MainMusic");


}


