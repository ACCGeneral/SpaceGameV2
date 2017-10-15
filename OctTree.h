#pragma once

#include "OBB.h"
#include <glm.hpp>
#include <memory>
#include "Octinfo.h"
#include <iostream>
#include "collisioninfo.h"

//our octtree that will help make collision detection alot cheaper and efficent (ray pointer needed for recursive problems)

bool insidecheck(glm::vec3 octmin, glm::vec3 octmax, glm::vec3 rayorigin);
bool IDcheck(Octinfo Octone, Octinfo Octtwo);

class OctTree 
{
private:

	glm::vec3 octmin;
	glm::vec3 octmax;
	glm::vec3 middle;

	OctTree *root;
	OctTree *child1;
	OctTree *child2;
	OctTree *child3;
	OctTree *child4;
	OctTree *child5;
	OctTree *child6;
	OctTree *child7;
	OctTree *child8;
	std::vector<Octinfo> objects;
	std::shared_ptr<collison> mycols;


public:

	OctTree(glm::vec3 minim, glm::vec3 maxim)
	{
		octmin = minim;
		octmax = maxim;
		glm::vec3 temp = octmax - octmin;
		middle = octmin + (temp / 2.0f);
	}

	void setcoldatabase(std::shared_ptr<collison> c)
	{
		mycols = c;
	}

	void construct(OctTree *parent, int divides);
	void removeentity(int ID);
	void getparententities(std::vector<Octinfo> &returnents);
	void collisiondetection();
	bool fitinoct(glm::vec3 objmin, glm::vec3 objmax, OctTree *lanode);
	void update();
	void addobject(Octinfo obj);
	void checkendts();

	std::vector<collisioninfo> returncolls()
	{
		return mycols->returncol();
	}

	raycollinfo returnraycol()
	{
		return mycols->returnrayinfo();
	}
	

	void clearcols()
	{
		mycols->clearall();
	}

	void cleanup();

	void removethis(unsigned long int id, bool &found);

	void getclosesttoray(Ray myray, std::vector<int> idstomiss, float maxdis);

	void getclosetome(glm::vec3 pos, float rad);

	bool somethingnearme(glm::vec3 pos, float maxlenght);

};