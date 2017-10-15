#pragma once

#include "scene.h"
#include <memory>
#include "TextManager.h"

//our class that will manage all of our scenes

class SceneManager : public std::enable_shared_from_this<SceneManager>
{
private:

	std::shared_ptr<scene> myscene;

public:

	SceneManager();
	~SceneManager();
	void changescene(std::shared_ptr<scene> S);
	void removescene();
	void addstate(std::shared_ptr<scene> S);

	void init();
	void draw(float deltatime, bool &go);
	void update(float deltatime, bool &go);

	std::shared_ptr<SceneManager> getself() {
		return shared_from_this();
	}


		
};
