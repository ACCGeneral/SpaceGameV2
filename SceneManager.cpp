#include "SceneManager.h"

SceneManager::SceneManager()
{
	myscene = NULL;
}

SceneManager::~SceneManager()
{


}

void SceneManager::changescene(std::shared_ptr<scene> S) // change the scene if we need to
{
	removescene();
	myscene = S;
	myscene->init();

}

void SceneManager::removescene() // remove our current scene
{
	if (myscene != NULL)
	{
		myscene->exit();
		myscene.reset();
	}
}

void SceneManager::addstate(std::shared_ptr<scene> S) // add a scene
{
	myscene = S;
}

void SceneManager::init() // the current scenes initialize 
{
	myscene->init();
}

void SceneManager::draw(float deltatime, bool &go) //draw the scene 
{
	myscene->draw(deltatime);
}

void SceneManager::update(float deltatime, bool &go) // update the scene 
{
	myscene->update(deltatime, go);
	
}

