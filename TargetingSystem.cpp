#include "TargetingSystem.h"

TargetingSystem::TargetingSystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "TargetingSystem";

}

void TargetingSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(targetable::TypeID))
		{
			targets.push_back(ent[i]);
		}
		else if (ent[i]->doihave(playercomp::TypeID) && player == NULL)
		{
			player = ent[i];
		}
		else if (ent[i]->doihave(trackingarrowscomp::TypeID) && trackerarrows == NULL)
		{
			trackerarrows = ent[i];
		}
	}

}

void TargetingSystem::update(float & dt, bool & go)
{
	if (player == NULL)
	{
		return;
	}


	std::shared_ptr<playercomp> playerinputs = player->getcomponent<playercomp>();
	std::shared_ptr<mytarget> playertarget = player->getcomponent<mytarget>();

	std::shared_ptr<trackingarrowscomp> trackingcomp = trackerarrows->getcomponent<trackingarrowscomp>();
	std::shared_ptr<modelcomponent> trackingmodel = trackerarrows->getcomponent<modelcomponent>();

	if (playerinputs->lookfortarget == true)
	{
		trackingcomp->targetbox = NULL;
		trackingmodel->drawme = false;

		if (playertarget->target != NULL)
		{
			std::shared_ptr<thingsiown> iown = playertarget->target->getcomponent<thingsiown>();
			for (auto ent : iown->stuffiown)
			{
				if (ent->returntype() == enttypes::Trackerbox || ent->returntype() == enttypes::UIBox)
				{
					std::shared_ptr<modelcomponent> entmodel = ent->getcomponent<modelcomponent>();
					uisettings *uiextra = static_cast<uisettings*>(entmodel->extra.release());
					uiextra->settarget(false);
					entmodel->extra = std::make_unique<uisettings>(*uiextra);
					delete uiextra;
				}
			}
		}

		std::shared_ptr<transposecomponent> playertrans = player->getcomponent<transposecomponent>();
		std::shared_ptr<directioncomponent> playerdir = player->getcomponent<directioncomponent>();
		std::shared_ptr<teamcomponent> playerteam = player->getcomponent<teamcomponent>();
		std::shared_ptr<ACC::entity> besttarget;
		float bestscore = -10000;

		for (auto ents : targets)
		{
			std::shared_ptr<teamcomponent> entteam = ents->getcomponent<teamcomponent>();
			std::shared_ptr<transposecomponent> entitytrans = ents->getcomponent<transposecomponent>();

			glm::vec3 angletest = glm::normalize(entitytrans->position - playertrans->position);
			float anglefloat = acos(glm::dot(angletest, playerdir->newdirect)) * 180.0f / 3.14f;

			if (!(fabs(anglefloat) > 40) && ents != player)
			{
				float maxdistance = 300;
				float distance = (maxdistance - glm::length(entitytrans->position - playertrans->position)) / maxdistance;
				float scoreangle = (40 - anglefloat) / 40 * 0.6;
				float score = (distance + scoreangle) / 2;

				if (playerteam->ally == entteam->ally)
				{
					score *= 0.6;
				}
				if (score > bestscore)
				{
					bestscore = score;
					playertarget->target = ents;
				}

			}

		}

		playerinputs->lookfortarget = false;
		if (playertarget->target != NULL)
		{
			std::shared_ptr<thingsiown> iown = playertarget->target->getcomponent<thingsiown>();
			for (auto ent : iown->stuffiown)
			{
				if (ent->returntype() == enttypes::Trackerbox || ent->returntype() == enttypes::UIBox)
				{
					if (ent->returntype() == enttypes::Trackerbox)
					{
						trackingcomp->targetbox = ent->getcomponent<transposecomponent>();
						trackingmodel->drawme = true;
					}
					std::shared_ptr<modelcomponent> entmodel = ent->getcomponent<modelcomponent>();
					uisettings *uiextra = static_cast<uisettings*>(entmodel->extra.release());
					uiextra->settarget(true);
					entmodel->extra = std::make_unique<uisettings>(*uiextra);
					delete uiextra;
				}
			}
		}
	}


	trackerarrowupdate();


}

void TargetingSystem::trackerarrowupdate()
{
	std::shared_ptr<trackingarrowscomp> trackingcomp = trackerarrows->getcomponent<trackingarrowscomp>();
	std::shared_ptr<transposecomponent> trackingtrans = trackerarrows->getcomponent<transposecomponent>();
	std::shared_ptr<transposecomponent> playertrans = player->getcomponent<transposecomponent>();

	if (trackingcomp->targetbox != NULL)
	{
		glm::vec3 newdir = playertrans->myquat * glm::vec3(0, 1, 0);

		glm::vec3 target = glm::normalize(glm::normalize(trackingcomp->targetbox->position) - glm::normalize(trackingtrans->position));

		glm::vec3 rightside = playertrans->myquat * glm::vec3(1, 0, 0);

		float side = glm::dot(target, rightside);

		float angle = acos(glm::dot(newdir, target));

		if (side < 0)
		{
			angle = -angle;
		}
		glm::vec3 mycross = playertrans->myquat * glm::vec3(0, 0, -1);
		glm::quat quadangletobox = glm::angleAxis(angle, mycross);
		glm::mat4 playerori = glm::toMat4(playertrans->myquat);
		glm::mat4 angletobox = glm::toMat4(quadangletobox);
		trackingtrans->position = playertrans->position + (playertrans->myquat * trackingcomp->offset);
		trackingtrans->myquat = glm::toQuat(angletobox * playerori);
		
	}
	else
	{
		std::shared_ptr<modelcomponent> trackingmodel = trackerarrows->getcomponent<modelcomponent>();
		trackingmodel->drawme = false;
	}



}

void TargetingSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < ents.size(); i++)
	{
		for (int j = 0; j < targets.size(); j++)
		{
			if (player != NULL)
			{
				std::shared_ptr<mytarget> playertarget = player->getcomponent<mytarget>();
				std::shared_ptr<trackingarrowscomp> trackingcomp = trackerarrows->getcomponent<trackingarrowscomp>();

				if (trackerarrows != NULL && playertarget->target == ents[i])
				{
					trackingcomp->targetbox = NULL;
					playertarget->target = NULL;
				}
			}

			if (targets[j] == ents[i])
			{
				targets.erase(targets.begin() + j);
				ents.erase(ents.begin() + i);
				j--;
				i--;
				break;
			}
			else if (ents[i] == player)
			{
				std::shared_ptr<mytarget> playertarget = player->getcomponent<mytarget>();

				if (playertarget->target != NULL)
				{
					std::shared_ptr<thingsiown> iown = playertarget->target->getcomponent<thingsiown>();
					for (auto ent : iown->stuffiown)
					{
						if (ent->returntype() == enttypes::Trackerbox || ent->returntype() == enttypes::UIBox)
						{
							std::shared_ptr<modelcomponent> entmodel = ent->getcomponent<modelcomponent>();
							uisettings *uiextra = static_cast<uisettings*>(entmodel->extra.release());
							uiextra->settarget(false);
							entmodel->extra = std::make_unique<uisettings>(*uiextra);
							delete uiextra;
						}
					}
				}

				playertarget->target = NULL;
				ents.erase(ents.begin() + i);
				i--;
				player = NULL;
				break;
			}
			else if (ents[i] == trackerarrows)
			{
				std::shared_ptr<trackingarrowscomp> trackingcomp = trackerarrows->getcomponent<trackingarrowscomp>();
				trackingcomp->targetbox = NULL;
				ents.erase(ents.begin() + i);
				i--;
				trackerarrows = NULL;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}

}

