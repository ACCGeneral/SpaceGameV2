#include "modelholder.h"
#include <thread>

modelholder::modelholder()
{
  
}

modelholder::~modelholder()
{


}

void modelholder::addmodel(std::string modelpath, std::string modelname) //add a new model to the models we access 
{
	std::shared_ptr<model> newmodel = std::make_shared<model>(GL_STATIC_DRAW);

	newmodel->loadmodel(modelpath);

	std::pair<std::shared_ptr<model>, std::string> newpair;

	newpair.first = newmodel;
	newpair.second = modelname;

	models.push_back(newpair);

	AABB newAABB;
	newAABB.setupabb(newpair.first->returnmymeshes());

	sphere newsphere(newAABB.min, newAABB.max);

	OBB newOBB;
	newOBB.obbsetup(newpair.first->returnmymeshes());

	std::pair<AABB, std::string> newpaira;
	newpaira.first = newAABB; 
	newpaira.second = modelname;
	modelaabbs.push_back(newpaira);

	std::pair<sphere, std::string> newspair;
	newspair.first = newsphere;
	newspair.second = modelname;
	modelsphere.push_back(newspair);

	std::pair<OBB, std::string> newpairo;
	newpairo.first = newOBB;
	newpairo.second = modelname;
	modelOBBs.push_back(newpairo);

}

std::shared_ptr<model> modelholder::getmode(std::string name) //get the model we want
{
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i].second == name)
		{
			return models[i].first;
		}
	}

	return NULL;


}

void modelholder::addskybox(std::string modelpath, std::vector<std::string> cube, std::string modelname) //add a skybox model to the model loader
{
	std::shared_ptr<model> skyboxmodel = std::make_shared<model>(GL_STATIC_DRAW);
	skyboxmodel->setskybox(true, cube);
	skyboxmodel->loadmodel(modelpath);

	std::pair<std::shared_ptr<model>, std::string> newpair;

	newpair.first = skyboxmodel;
	newpair.second = modelname;

	models.push_back(newpair);

}



AABB modelholder::getAABB(std::string name) //get the aabb generated with the model 
{
	for (int i = 0; i < modelaabbs.size(); i++)
	{
		if (modelaabbs[i].second == name)
		{
			return modelaabbs[i].first;
		}
	}

	return AABB();
}

OBB modelholder::GetOBB(std::string name)
{
	for (int i = 0; i < modelOBBs.size(); i++)
	{
		if (modelaabbs[i].second == name)
		{
			return modelOBBs[i].first;
		}
	}

	return OBB();
}

sphere modelholder::getsphere(std::string name)
{
	for (int i = 0; i < modelsphere.size(); i++)
	{
		if (modelaabbs[i].second == name)
		{
			return modelsphere[i].first;
		}
	}


	return sphere();
}

