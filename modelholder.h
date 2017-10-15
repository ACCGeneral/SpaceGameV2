#pragma once

#include <memory>
#include "model.h"
#include <vector>
#include <string>
#include "OBB.h"
#include "AABB.h"
#include "Sphere.h"

class modelholder // this is the class that will contain all of the models within the game
{
public:

  modelholder();
  ~modelholder();

  void addmodel(std::string modelpath, std::string modelname);
  std::shared_ptr<model> getmode(std::string name);
  void addskybox(std::string modelpath,std::vector<std::string> cube, std::string modelname);

  AABB getAABB(std::string name);
  OBB GetOBB(std::string name);
  sphere getsphere(std::string name);
	  
private:

	std::vector<std::pair<std::shared_ptr<model>, std::string>> models;
	std::vector<std::pair<AABB, std::string>> modelaabbs;
	std::vector<std::pair<OBB, std::string>> modelOBBs;
	std::vector<std::pair<sphere, std::string>> modelsphere;
};