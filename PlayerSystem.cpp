#include "PlayerSystem.h"
#include "maincamera.h"

PlayerSystem::PlayerSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "PlayerSystem";
}

void PlayerSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(playercomp::TypeID) && player == NULL)
		{
			player = ent[i];
			std::shared_ptr<transposecomponent> playertrans = ent[i]->getcomponent<transposecomponent>();
			myworld->getmaincam()->getplayerpos(playertrans);
		}
	}


}

void PlayerSystem::update(float & dt, bool & go)
{
	if (player == NULL)
	{
		return;
	}

	std::shared_ptr<transposecomponent> playertrans = player->getcomponent<transposecomponent>();
	std::shared_ptr<directioncomponent> playerdir = player->getcomponent<directioncomponent>();
	std::shared_ptr<physics> playerphysics = player->getcomponent<physics>();
	std::shared_ptr<playercomp> playerc = player->getcomponent<playercomp>();
	std::shared_ptr<fighterguns> playerguns = player->getcomponent<fighterguns>();

	if (inputlogger::instance().getkeydown(SDLK_t))
	{
		playerc->lookfortarget = true;
	}

	if (inputlogger::instance().getleftmouse() == true && playerguns->currettime >= playerguns->firetime)
	{
		playerguns->fire = true;
	}

	glm::vec3 rotation = glm::vec3(0,0,0);
	glm::vec3 currentmouse = glm::vec3(0, 0, 0);

	currentmouse.x = inputlogger::instance().getmousex();
	currentmouse.y = inputlogger::instance().getmousey();
	currentmouse.z = 0;

	if (currentmouse.x < 540)
	{
		rotation.y = -(540 - currentmouse.x) * 0.002f * dt;
	}
	else if (currentmouse.x > 740)
	{
		rotation.y = (currentmouse.x - 720) * 0.002f * dt;
	}
	else
	{
		rotation.y = 0.0f;
	}

	if (currentmouse.y > 400)
	{
		rotation.x = (currentmouse.y - 400) * 0.004f * dt;
	}
	else if (currentmouse.y < 320)
	{
		rotation.x = -(320 - currentmouse.y) * 0.004f * dt;
	}
	if (inputlogger::instance().getkeydown(SDLK_d))
	{
		rotation.z = (100.0f) * 0.009f * dt;
	}
	if (inputlogger::instance().getkeydown(SDLK_a))
	{
		rotation.z = -(100.0f) * 0.009f * dt;
	}


	glm::quat quatx = glm::angleAxis((rotation.x), playerc->right);

	//rotate the up and forward axis from this new rotation

	playerc->up = quatx * playerc->up;
	playerc->forward = quatx * playerc->forward;

	//same for y axis and angle

	glm::quat quaty = glm::angleAxis((rotation.y), playerc->up);

	playerc->forward = quaty * playerc->forward;
	playerc->right = quaty * playerc->right;

	//same for z axis and angle

	glm::quat quatz = glm::angleAxis((rotation.z), playerc->forward);

	playerc->up = quatz * playerc->up;
	playerc->right = quatz * playerc->right;

	//periodic re-ortho-normalization is needed to correct our axes

	playerc->up = glm::cross(playerc->right, playerc->forward);
	playerc->right = glm::cross(playerc->forward, playerc->up);
	playerc->forward = playerc->forward;

	//re-normalize the 3 axes for safe mesure

	playerc->up = glm::normalize(playerc->up);
	playerc->right = glm::normalize(playerc->right);
	playerc->forward = glm::normalize(playerc->forward);

	//create our new rotation from our rotation * xquat * yquat * zquat

	playertrans->myquat = quatz * quaty * quatx * playertrans->myquat;

	playertrans->myquat = glm::normalize(playertrans->myquat);

	playerdir->newdirect = playertrans->myquat * playerdir->direction;

	if (inputlogger::instance().getkeydown(SDLK_w) && playerphysics->speed < playerphysics->maxspeed)
	{
		playerphysics->speed += 0.2;
	}
	if (inputlogger::instance().getkeydown(SDLK_s) && playerphysics->speed > 0)
	{
		playerphysics->speed -= 0.2;
	}

	playerphysics->velocity += (playerdir->newdirect * playerphysics->speed);

	if (glm::length(playerphysics->velocity) > playerphysics->speed)
	{
		playerphysics->velocity = glm::normalize(playerphysics->velocity) * playerphysics->speed;
	}

	myworld->getmaincam()->setdirection(playerdir->newdirect);

	glm::vec3 newsoundlocation = playertrans->position + (playerphysics->velocity) * dt;
	alListener3f(AL_POSITION, newsoundlocation.x, newsoundlocation.y, newsoundlocation.z);

	
}

void PlayerSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < ents.size(); i++)
	{
		if (player == ents[i])
		{
			player = NULL;
			break;
		}
	}


}
