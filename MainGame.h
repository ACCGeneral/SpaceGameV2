#pragma once

#include "scene.h"
#include "TextManager.h"
#include "spawnsystem.h"
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

class MainGame : public scene
{
private:

	std::shared_ptr<world> myworld;
	std::shared_ptr<ACC::System_Manager> MyManager;
	spawnsystem myspawnsystem;

	std::shared_ptr<healthcomponent> allycapship;
	std::shared_ptr<healthcomponent> enemycapship;

	float wantedFPS;

	sound mainsound;

public:


	MainGame(std::shared_ptr<SceneManager> Sman, std::shared_ptr<entitymanager> myman);
	void init();
	void update(float dt, bool &go);
	void draw(float dt);
	void exit();

};