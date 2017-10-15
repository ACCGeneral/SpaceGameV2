#include "FireSystem.h"

FireSystem::FireSystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "FireSystem";

}

void FireSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(fighterguns::TypeID))
		{
			gunobjects.push_back(ent[i]);
		}
	}

}

void FireSystem::update(float & dt, bool & go)
{
	//turret guns fix?
	for (auto num : gunobjects)
	{
		std::shared_ptr<fighterguns> myguns = num->getcomponent<fighterguns>();
		if (myguns != NULL)
		{
			myguns->currettime += dt;

			if (myguns->fire == true && myguns->currettime > myguns->firetime)
			{
				std::shared_ptr<transposecomponent> entitytrans = num->getcomponent<transposecomponent>();
				std::shared_ptr<directioncomponent> entitydir = num->getcomponent<directioncomponent>();
				std::shared_ptr<teamcomponent> teamcomp = num->getcomponent<teamcomponent>();

					for (int j = 0; j < myguns->guns.size(); j++)
					{
						projectileinfo projectile;
						projectile.team = teamcomp->Team;
						projectile.dir = entitydir->newdirect;
						projectile.dam = myguns->damage;
						projectile.pos = entitytrans->position;
						projectile.pos += (entitytrans->myquat * myguns->guns[j]);
						projectile.rota = entitytrans->myquat;
						projectile.speed = myguns->speed;
						if (myguns->Wtype == myguns->bullet)
						{
							myworld->returnmanager()->createbullet(projectile);
						}
						else if (myguns->Wtype == myguns->bomb)
						{
							myworld->returnmanager()->createbomb(projectile);
						}
						myguns->fire = false;

						myguns->firesound.setpos(projectile.pos);
						myguns->firesound.playsound();

					}
					myguns->currettime = 0;
			}
		}
	}

}

void FireSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < gunobjects.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (gunobjects[i] == ents[j])
			{
				gunobjects.erase(gunobjects.begin() + i);
				ents.erase(ents.begin() + j);
				j--;
				i--;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}

}
