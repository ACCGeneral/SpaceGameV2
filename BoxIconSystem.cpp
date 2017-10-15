#include "BoxIconSystem.h"
#include "maincamera.h"

BoxIconSystem::BoxIconSystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "BoxIconSystem";
}

void BoxIconSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(uibox::TypeID))
		{
			boxobjects.push_back(ent[i]);
		}
		else if (ent[i]->doihave(playercomp::TypeID) && player == NULL)
		{
			player = ent[i];
		}
	}

}

void BoxIconSystem::update(float & dt, bool & go)
{
	if (player == NULL)
	{
		return;
	}

	std::shared_ptr<transposecomponent> playertrans = player->getcomponent<transposecomponent>();

	for (auto ent : boxobjects)
	{
		std::shared_ptr<uibox> box = ent->getcomponent<uibox>();
		std::shared_ptr<transposecomponent> boxtrans = ent->getcomponent<transposecomponent>();

		std::shared_ptr<modelcomponent> boxmodel = ent->getcomponent<modelcomponent>();

		std::shared_ptr<transposecomponent> ownertrans = box->myship->getcomponent<transposecomponent>();

		glm::vec3 campos = myworld->getmaincam()->getpos();

		float distancefromcamera = fabs(glm::length(campos - ownertrans->position));

		float test = distancefromcamera - (distancefromcamera * 0.70);

		if (test > box->maxsizebox)
		{
			box->nowsize = box->maxsizebox;
		}
		else
		{
			box->nowsize = std::fmin((test / box->maxsizebox), 1) * box->maxsizebox;

			if (box->nowsize < 1)
			{
				box->nowsize = 1;
			}
		}

		boxtrans->position = ownertrans->position;
		glm::vec3 scalevec = glm::vec3(1, 1, 1) * box->nowsize;
		boxtrans->scale = scalevec;
		uisettings *uiextra = static_cast<uisettings*>(boxmodel->extra.release());
		uiextra->setscale(scalevec);
		boxmodel->extra = std::make_unique<uisettings>(*uiextra);
		delete uiextra;
		boxtrans->myquat = playertrans->myquat;
	}
}

void BoxIconSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < ents.size(); i++)
	{
		for (int j = 0; j < boxobjects.size(); j++)
		{
			if (boxobjects[j] == ents[i])
			{
				boxobjects.erase(boxobjects.begin() + j);
				ents.erase(ents.begin() + i);
				j--;
				i--;
				break;
			}

			if (ents[i] == player)
			{
				ents.erase(ents.begin() + i);
				i--;
				player = NULL;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}


}
