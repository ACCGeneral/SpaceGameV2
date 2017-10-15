#pragma once

//collision class that will keep track of all collisions that happen in a frame

struct collisioninfo
{
	unsigned long int ID1;
	unsigned long int ID2;

	glm::vec3 projectionaxis;
	float overlap;
};

struct raycollinfo
{
	glm::vec3 hitpoint;
	glm::vec3 center;
	float t;
	bool hit;
	unsigned long int closestID;
};

struct nearmeinfo
{
	std::vector<unsigned long int> IDsnearme;
};


class collison
{
private:
	std::vector<collisioninfo> normalcollion;
	raycollinfo raycol;
	nearmeinfo nearme;

public:

	void addcol(collisioninfo newcol)
	{
		normalcollion.push_back(newcol);
	}
	std::vector<collisioninfo> returncol()
	{
		return normalcollion;
	}
	void clearall()
	{
		normalcollion.clear();
	}
	void addidnear(int id)
	{
		nearme.IDsnearme.push_back(id);
	}
	nearmeinfo returnnear()
	{
		return nearme;
	}

	void setraycol(glm::vec3 HitP, int ID, int newt, glm::vec3 cen)
	{
		if (newt < raycol.t)
		{
			raycol.closestID = ID;
			raycol.hitpoint = HitP;
			raycol.t = newt;
			raycol.center = cen;
			raycol.hit = true;
		}
	}

	raycollinfo returnrayinfo()
	{
		return raycol;
	}

	void clearnearme()
	{
		nearme.IDsnearme.clear();
	}

	void resetraycol()
	{
		raycol.hit = false;
		raycol.t = 1000000;
		raycol.hitpoint = glm::vec3(-1, -1, -1);
	}
};