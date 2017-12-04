#include "Trackingboxsystem.h"
#include "maincamera.h"

Trackingboxsystem::Trackingboxsystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "Trackingboxsystem";
}

void Trackingboxsystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(trackerbox::TypeID))
		{
			trackingbox.push_back(ent[i]);
		}
		else if (ent[i]->doihave(playercomp::TypeID) && player == NULL)
		{
			player = ent[i];
		}
	}

}

void Trackingboxsystem::update(float & dt, bool & go)
{

	std::shared_ptr<mytarget> playertarget;
	std::shared_ptr<transposecomponent> playertrans;

	if (player != NULL)
	{
		 playertarget = player->getcomponent<mytarget>();
		 playertrans = player->getcomponent<transposecomponent>();
	}
	else
	{
		return;
	}
	

	for (auto ent : trackingbox)
	{
		std::shared_ptr<trackerbox> trackbox = ent->getcomponent<trackerbox>();
		std::shared_ptr<transposecomponent> boxtrans = ent->getcomponent<transposecomponent>();
		std::shared_ptr<modelcomponent> boxmod = ent->getcomponent<modelcomponent>();

		std::shared_ptr<transposecomponent> ownertrans = trackbox->unit->getcomponent<transposecomponent>();

		glm::vec3 campos = myworld->getmaincam()->getpos();
		glm::vec3 camdir = myworld->getmaincam()->getdir();

		glm::vec3 between = glm::normalize(ownertrans->position - campos);

		float angle = glm::degrees(abs(glm::acos(glm::dot(between, camdir))));

		if (angle < trackbox->anglecutoff && playertarget->target != trackbox->unit)
		{
			boxmod->drawme = false;
		}
		else
		{
			glm::vec3 normalshippos = glm::normalize(ownertrans->position - campos);
			glm::vec3 difference;

			difference = glm::normalize(normalshippos - camdir);

			float roll = glm::degrees(glm::roll(playertrans->myquat));

			glm::quat zrot = glm::angleAxis(glm::radians(-roll), glm::vec3(0, 0, 1));

			float pitch = glm::degrees(glm::pitch(playertrans->myquat));

			glm::quat xrot = glm::angleAxis(glm::radians(-pitch), glm::vec3(1, 0, 0));

			float yaw = glm::degrees(glm::yaw(playertrans->myquat));

			glm::quat yrot = glm::angleAxis(glm::radians(-yaw), glm::vec3(0, 1, 0));

			glm::vec3 boxup = playertrans->myquat * glm::vec3(0, 1, 0);

			glm::vec3 boxdown = playertrans->myquat * -glm::vec3(0, 1, 0);

			glm::vec3 boxleft = playertrans->myquat * -glm::vec3(1, 0, 0);

			glm::vec3 boxright = playertrans->myquat * glm::vec3(1, 0, 0);

			float boxleftangletest2 = glm::degrees(acos(glm::dot(boxleft, difference)));

			float boxrightangletest2 = glm::degrees(acos(glm::dot(boxright, difference)));

			float boxupangletest2 = glm::degrees(acos(glm::dot(boxup, difference)));

			float boxdownangletest2 = glm::degrees(acos(glm::dot(boxdown, difference)));

			float minangle = std::fmin(std::fmin(std::fmin(boxleftangletest2, boxrightangletest2), boxupangletest2), boxdownangletest2);

			if (minangle == boxleftangletest2)
			{
				glm::quat leftquad = glm::angleAxis(glm::radians(40.3f), playertrans->myquat * -glm::vec3(0, 1, 0));
				glm::vec3 leftray = (leftquad * camdir) * 40.0f;
				boxtrans->position = leftray + campos;

				difference = zrot * difference;
				difference = yrot * difference;
				difference = xrot * difference;

				float ydiff = 29.0f * difference.y;
				ydiff = std::fmin(13.5f, ydiff);
				ydiff = std::fmax(-13.5f, ydiff);

				boxtrans->position += boxup * ydiff;

			}
			else if (minangle == boxrightangletest2)
			{
				glm::quat rightquad = glm::angleAxis(glm::radians(40.3f), playertrans->myquat * glm::vec3(0, 1, 0));
				glm::vec3 rightray = (rightquad * camdir) * 40.0f;
				boxtrans->position = rightray + campos;

				difference = zrot * difference;
				difference = yrot * difference;
				difference = xrot * difference;

				float ydiff = 29.0f * difference.y;
				ydiff = std::fmin(13.5f, ydiff);
				ydiff = std::fmax(-13.5f, ydiff);

				boxtrans->position += boxup * ydiff;

			}
			else if (minangle == boxupangletest2)
			{
				glm::quat upquad = glm::angleAxis(glm::radians(25.5f), playertrans->myquat * -glm::vec3(1, 0, 0));
				glm::vec3 upray = (upquad * camdir) * 40.0f;
				boxtrans->position = upray + campos;

				difference = zrot * difference;
				difference = yrot * difference;
				difference = xrot * difference;

				float xdiff = 31.0f * difference.x;
				boxtrans->position += boxright * xdiff;

			}
			else if (minangle == boxdownangletest2)
			{
				glm::quat downquad = glm::angleAxis(glm::radians(25.5f), playertrans->myquat * glm::vec3(1, 0, 0));
				glm::vec3 downray = (downquad * camdir) * 40.0f;
				boxtrans->position = downray + campos;

				difference = zrot * difference;
				difference = yrot * difference;
				difference = xrot * difference;

				float xdiff = 31.0f * difference.x;
				boxtrans->position += boxright * xdiff;

			}

			if (angle < trackbox->anglecutoff)
			{
				boxmod->drawme = false;

			}
			else
			{
				boxmod->drawme = true;
			}
		}
	}

	

}

void Trackingboxsystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < ents.size(); i++)
	{
		for (int j = 0; j < trackingbox.size(); j++)
		{
			if (trackingbox[j] == ents[i])
			{
				trackingbox.erase(trackingbox.begin() + j);
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


