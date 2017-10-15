#pragma once

#include "scene.h"
#include "TextManager.h"
#include "spawnsystem.h"


class MainGame : public scene
{
private:

	std::shared_ptr<world> myworld;
	std::shared_ptr<ACC::System_Manager> MyManager;
	spawnsystem myspawnsystem;

	std::shared_ptr<healthcomponent> allycapship;
	std::shared_ptr<healthcomponent> enemycapship;

	float wantedFPS;

public:


	MainGame(std::shared_ptr<SceneManager> Sman, std::shared_ptr<entitymanager> myman);
	void init();
	void update(float dt, bool &go);
	void draw(float dt);
	void exit();

};