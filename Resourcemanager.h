#pragma once

#include "shaderholder.h"
#include "modelholder.h"
#include "OBBholder.h"
#include "soundholder.h"

//our resource manager for all shaders,models and textures. 

class Resourecmanager
{
private:
	shaderholder shadholder;
	modelholder modholder;
	OBBholder myOBBholder;
	soundholder mysoundholder;

public:

	static Resourecmanager &instance();

	std::shared_ptr<shader> getfromshadholder(std::string name);
	std::shared_ptr<model> getfrommodelholder(std::string name);
	AABB getAABB (std::string name);
	OBB getOBBfrommodel(std::string name);
	sphere getsphere(std::string name);
	std::vector<OBB> getOBB(std::string name);
	sound getsound(std::string name);


	void addshader(std::shared_ptr<shader> newshader,std::string name);
	void addmodels(std::string modelpath, std::string modelname);
	void addskybox(std::vector<std::string> cube, std::string modelpath, std::string modelname);
	void abbobb(std::string filename, std::string name);
	void addsound(sound newsound, std::string name);


	void modelsinit();
	void obbinit();
	void shaderinit();
	void soundsinit();

};