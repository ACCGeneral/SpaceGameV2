#include "RenderSystem.h"
#include "maincamera.h"

RenderSystem::RenderSystem(std::shared_ptr<world> w, std::shared_ptr<EventSystem> EventSys) : System(w, EventSys)
{
	my_System_Name = "RenderSystem";

}

void RenderSystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<unsigned long int> lookfor;
	lookfor.push_back(transposecomponent::TypeID);
	lookfor.push_back(modelcomponent::TypeID);

	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(lightcomp::TypeID))
		{
			lights.push_back(ent[i]);
		}

		if (ent[i]->doihave(lookfor))
		{
			std::shared_ptr<modelcomponent> entitymodel = ent[i]->getcomponent<modelcomponent>();
			if (entitymodel->depth == true)
			{
				modelents.push_back(ent[i]);
			}
			else
			{
				backdraw.push_back(ent[i]);
			}

		}
	}


}

void RenderSystem::update(float & dt, bool & go) //render all of our entities 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int spotlightnum = 0;
	int dirlightnum = 0;
	int pointlightnum = 0;

	std::vector<std::shared_ptr<ACC::entity>> entstorender;
	entstorender.insert(entstorender.end(), modelents.begin(), modelents.end()); // normal entities render first 
	entstorender.insert(entstorender.end(), backdraw.begin(), backdraw.end()); //render no depth entities last (drawing order matters for these)

	for (auto models : entstorender)
	{
		spotlightnum = 0;
		dirlightnum = 0;
		pointlightnum = 0;
		std::shared_ptr<transposecomponent> entitytrans = models->getcomponent<transposecomponent>();
		std::shared_ptr<modelcomponent> entitymodel = models->getcomponent<modelcomponent>(); 

		if (entitymodel->drawme == true)
		{
			entitymodel->myshader->useProgram();
			if (entitymodel->effectedbylights == true) //link all of the lights to the shader if needed
			{
				for (auto L : lights)
				{
					std::shared_ptr<lightcomp> light = L->getcomponent<lightcomp>();
					if (light->mylight->returnlighttype() == DLight)
					{
						light->mylight->shaderlinks(entitymodel->myshader->returnprogram(), std::to_string(dirlightnum));
						dirlightnum++;
					}
					else if (light->mylight->returnlighttype() == PLight)
					{
						light->mylight->shaderlinks(entitymodel->myshader->returnprogram(), std::to_string(pointlightnum));
						pointlightnum++;
					}
					else
					{
						light->mylight->shaderlinks(entitymodel->myshader->returnprogram(), std::to_string(spotlightnum));
						spotlightnum++;
					}
				}

				glm::vec3 campos = myworld->getmaincam()->getpos();

				glUniform3f(glGetUniformLocation(entitymodel->myshader->returnprogram(), "camerapos"), campos.x, campos.y, campos.z);
				glUniform1f(glGetUniformLocation(entitymodel->myshader->returnprogram(), "strength"), entitymodel->mystrenght);
				glUniform1f(glGetUniformLocation(entitymodel->myshader->returnprogram(), "shine"), entitymodel->myshine);
			}

			glUniformMatrix4fv(glGetUniformLocation(entitymodel->myshader->returnprogram(), "view"), 1, GL_FALSE, glm::value_ptr(myworld->getmaincam()->getview())); //link model view projection matrix
			glUniformMatrix4fv(glGetUniformLocation(entitymodel->myshader->returnprogram(), "projection"), 1, GL_FALSE, glm::value_ptr(myworld->getmaincam()->getproj()));
			glUniformMatrix4fv(glGetUniformLocation(entitymodel->myshader->returnprogram(), "model"), 1, GL_FALSE, glm::value_ptr(entitytrans->modelMatrix));

			if (entitymodel->extra != NULL)
			{
				entitymodel->extra->setupshader(entitymodel->myshader->returnprogram());
			}

			if (!entitymodel->depth)
			{
				glDisable(GL_DEPTH_TEST); // turn off depth if needed 
			}

			entitymodel->mymodel->draw(entitymodel->myshader); //draw our model

			entitymodel->myshader->stopProgram(); //unlink shader 
		}

	}

	glEnable(GL_DEPTH_TEST); //reset

}

void RenderSystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < lights.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (lights[i] == ents[j])
			{
				modelents.erase(lights.begin() + i);
				i--;
				break;
			}
		}
	}


	for (int i = 0; i <	modelents.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (modelents[i] == ents[j])
			{
				modelents.erase(modelents.begin() + i);
				ents.erase(ents.begin() + j);
				j--;
				i--;
				break;
			}
		}

		if (ents.size() == 0)
		{
			return;
		}
	}


	for (int i = 0; i < backdraw.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (backdraw[i] == ents[j])
			{
				backdraw.erase(backdraw.begin() + i);
				ents.erase(ents.begin() + j);
				j--;
				i--;
				break;
			}
		}

		if (ents.size() == 0)
		{
			return;
		}
	}


}
