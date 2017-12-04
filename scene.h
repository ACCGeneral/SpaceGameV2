#pragma once

#include "world.h"
#include "system_manager.h"
#include "inputlogger.h"

//the base class for our scenes 

class SceneManager;
class RenderSystem;
class EventSystem;

class scene : public std::enable_shared_from_this<scene>
{
private:

protected:

	std::shared_ptr<world> myworld;
	std::shared_ptr<SceneManager> myscenemanager;
	std::shared_ptr<EventSystem> EventManager;

	std::shared_ptr<RenderSystem> myRenderSystem;
	std::vector<std::shared_ptr<ACC::System>> myupdatesystems;

	void updateEnts();
	void destroyEnts();

public:

	scene(std::shared_ptr<SceneManager> SM, std::shared_ptr<EventSystem> ES);
	virtual void init() = 0;
	virtual void update(float dt, bool &go) = 0;
	virtual void draw(float dt) = 0;
	virtual void exit() = 0;

	std::shared_ptr<scene> getself() {
		return shared_from_this();
	}

};