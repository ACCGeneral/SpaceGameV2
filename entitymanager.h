#pragma once

#include "entity.h"
#include "ID_manager.h"
#include "dirlight.h"
#include "spotlight.h"
#include <gtc/matrix_transform.hpp> 
#include <memory>
#include "Resourcemanager.h"
#include <unordered_map>
#include "projecttileinfo.h"
#include "healthsettings.h"
#include "orthographicsettings.h"
#include "uisettings.h"
#include "particleExplotionData.h"


// the entity manager for creating and holding entities
class entitymanager
{
private:
	ID_manager IDs;

	std::unordered_map<unsigned long int,std::shared_ptr<ACC::entity>> entities;
	std::vector<std::shared_ptr<ACC::entity>> tooadd;
	std::vector<std::shared_ptr<ACC::entity>> todelete;

public:

	void create_skybox();
	void create_directionlight(std::shared_ptr<dirlight> mydirinfo);
	void createflashlight(std::shared_ptr<ACC::entity> parent);
	void createplayer(glm::vec3 pos);

	void createEnemyFighter(glm::vec3 pos);
	void createEnemyBomber(glm::vec3 pos);
	std::shared_ptr<healthcomponent> createEnemyCapital(glm::vec3 pos);
	
	void createAllyFighter(glm::vec3 pos);
	void createAllyBomber(glm::vec3 pos);
	std::shared_ptr<healthcomponent> createAllyCapital(glm::vec3 pos);

	void createCapTurret(std::shared_ptr<ACC::entity> mycap,bool team, glm::vec3 offset);
	void createTurretGuns(std::shared_ptr<ACC::entity> myturret,bool team);

	void createcrosshair(std::shared_ptr<ACC::entity> player);

	void createbullet(projectileinfo info);
	void createbomb(projectileinfo info);

	void createUIbox(std::shared_ptr<ACC::entity> myent, int maxscale, bool team);
	void trackerboxent(std::shared_ptr<ACC::entity> myent, bool team);
	void createdirarrow(std::shared_ptr<ACC::entity> myent);
	void healthbar(std::shared_ptr<ACC::entity> myent, glm::vec3 size, glm::vec3 offset, bool playerhealth);

	void createcapicon(glm::vec3 maxscale, bool ally, std::shared_ptr<ACC::entity> player);

	void createsound(std::shared_ptr<soundComponet> soundcomp);

	void createExplotionParticleEffect(particleExplotionData explotiondata);

	entitymanager();

	void delete_entity();

	std::vector<std::shared_ptr<ACC::entity>> returntoadd()
	{
		return tooadd;
	}

	std::vector<std::shared_ptr<ACC::entity>> returntodelete()
	{
		return todelete;
	}

	void cleartoadd()
	{
		tooadd.clear();
	}

	void cleartodelete();

   std::unordered_map<unsigned long int,std::shared_ptr<ACC::entity>> returnentities() // return all our entites
   {
	  return entities;
   }

   std::shared_ptr<ACC::entity> getentfromid(unsigned long int ID)
   {
	   return entities[ID];
   }

   void addtodeletelist(std::shared_ptr<ACC::entity> deleteme)
   {
	   deleteme->setdeleteme();
	   todelete.push_back(deleteme);
   }

};
