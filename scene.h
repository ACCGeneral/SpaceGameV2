#pragma once

#include "world.h"
#include "system_manager.h"
#include "inputlogger.h"

//the base class for our scenes 

class SceneManager;

class scene : public std::enable_shared_from_this<scene>
{
private:

protected:

	std::shared_ptr<SceneManager> myscenemanager;

public:

	scene(std::shared_ptr<SceneManager> SM);
	virtual void init() = 0;
	virtual void update(float dt, bool &go) = 0;
	virtual void draw(float dt) = 0;
	virtual void exit() = 0;

	std::shared_ptr<scene> getself() {
		return shared_from_this();
	}

};