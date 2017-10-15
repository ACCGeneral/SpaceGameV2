#pragma once

#include <glm.hpp>
#include "glew.h"
#include <vector>
#include "shader.h"
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "Light.h"
#include "dirlight.h"
#include <memory>
#include "model.h"
#include "OBB.h"
#include "AABB.h"
#include "shadersettings.h"
#include "action.h"
#include "Sphere.h"
#include "sound.h"
#include "movementcontroller.h"

//here is a list of all of our components that we need for the game and the demo. every component is based of the base class ACC:component 
//For use components only contain data while systems do the needed logic

namespace ACC 
{

	class entity;
 
	class component 
	{

	public:

		virtual int GetType() const = 0;
		
	};

}

// typeID is used for getting the component from the entity vector list

class transposecomponent : public ACC::component
{

public:

	static const int TypeID = 1;

	int GetType() const { return TypeID; };

  glm::mat4 modelMatrix;

  glm::vec3 position;

  glm::quat myquat;

  glm::vec3 scale;

};



class directioncomponent : public ACC::component
{
 public:

	 static const int TypeID = 2;

	 int GetType() const  { return TypeID; };

   glm::vec3 direction;

   glm::vec3 newdirect;

};

class modelcomponent : public ACC::component
{
public:

	static const int TypeID = 3;

	int GetType() const { return TypeID; };

	std::shared_ptr<model> mymodel;
	std::shared_ptr<shader> myshader;
	std::unique_ptr<shadersettings> extra;

	bool drawme;
	bool effectedbylights;
	float mystrenght;
	float myshine;
	bool depth;

};


class lightcomp : public ACC::component
{
public:


	static const int TypeID = 4;

	int GetType() const { return TypeID; };

	std::shared_ptr<light> mylight;

};


class healthcomponent : public ACC::component
{

public:

	healthcomponent()
	{
		deathsound.second = false;
	}

	static const int TypeID = 5;

	float health;

	float maxhp;

	std::pair<sound,bool> deathsound;

	std::pair<bool,bool> hitdelete;

	int GetType() const { return TypeID; };

};

class animationholdercomp : public ACC::component
{

public:
	static const int TypeID = 6;

	enum type { light, trans };

	type mytype;

	std::shared_ptr<ACC::entity> myowner;

	glm::vec3 offset;

	glm::quat myanimation;

	int GetType() const { return TypeID; };

};

class playercomp : public ACC::component
{

public:
	static const int TypeID = 7;

	glm::vec3 rotation;
	glm::vec2 currentmouse;
	bool freezecam;
	bool lookfortarget;

	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	playercomp()
	{
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);
		forward = glm::vec3(0, 0, 1);
	}

	int GetType() const { return TypeID; };

};

class AIcomp : public ACC::component
{
	
public:

	static const int TypeID = 8;

	int GetType() const { return TypeID; };

	bool free;

	std::unique_ptr<action> myaction;

	float score;

	movementcontroller MovCon;

	AIcomp()
	{
		score = 0;
		free = true;
	}

};

class physics : public ACC::component
{

public:

	static const int TypeID = 9;

	float mass;
	float maxvelocity;
	glm::vec3 velocity;
	float speed;
	float maxforce;
	glm::vec3 force;
	bool physicsobject;

	int GetType() const { return TypeID; };

};

class collisioncomp : public ACC::component
{

public:

	static const int TypeID = 10;

	std::vector<std::shared_ptr<OBB>> myobb;
	std::shared_ptr<AABB> myAABB;
	std::shared_ptr<sphere> mysphere;
	bool imstatic;
	bool physicseffected;
	std::vector<unsigned long int> dontcolwith;

	int GetType() const { return TypeID; };
};

class damagedelt : public ACC::component
{

public:

	static const int TypeID = 11;

	sound damagesound;
	float damage;
	float maxdamage;

	int GetType() const { return TypeID; };
}; 



class uibox : public ACC::component
{

public:

	static const int TypeID = 12;

	unsigned int maxsizebox;

	std::shared_ptr<ACC::entity> myship;

	float nowsize;

	int GetType() const
	{
		return TypeID;
	}

};

class crosshaircomp : public ACC::component
{

public:

	static const int TypeID = 13;

	std::shared_ptr<ACC::entity> owner;

	int GetType() const { return TypeID; };
};



class turretcomp : public ACC::component
{

public:

	static const int TypeID = 14;

	glm::vec3 basedir;

	int GetType() const
	{
		return TypeID;
	}

};



class teamcomponent : public ACC::component
{

public:

	static const int TypeID = 15;

	std::string Team;

	bool ally;

	int GetType() const
	{
		return TypeID;
	}

};

class manuvervector : public ACC::component
{

public:

	static const int TypeID = 16;

	std::vector<glm::vec3> manuv;
	 
	int GetType() const
	{
		return TypeID;
	}

};

class owner : public ACC::component
{

public:

	static const int TypeID = 17;
	std::shared_ptr<ACC::entity> myowner;

	int GetType() const
	{
		return TypeID;
	}

};

class fighterguns : public ACC::component
{


public:

	enum weapontype{bullet, bomb};

	static const int TypeID = 18;

	std::vector<glm::vec3> guns;

	unsigned int numberofguns;

	float damage;

	bool fire;

	float firetime;
	float currettime;
	float speed;

	weapontype Wtype;

	sound firesound;

	int GetType() const
	{
		return TypeID;
	}
};

class lifespan : public ACC::component
{

public:

	static const int TypeID = 19;

	float lifetime;

	int GetType() const
	{
		return TypeID;
	}

};

class trackerbox : public ACC::component
{

public:

	static const int TypeID = 20;

	float anglecutoff;

	std::shared_ptr<ACC::entity> unit;

	int GetType() const
	{
		return TypeID;
	}

};


class unithealthbar : public ACC::component
{

public:

	static const int TypeID = 21;

	std::shared_ptr<ACC::entity> unit;

	glm::vec3 Doffset;

	bool playerhealth;

	int GetType() const
	{
		return TypeID;
	}

};

class targetable : public ACC::component
{
public:

	static const int TypeID = 22;

	targetable()
	{
	
	}

	int GetType() const
	{
		return TypeID;
	}

};


class thingsiown : public ACC::component
{

public:

	static const int TypeID = 23;

	std::vector<std::shared_ptr<ACC::entity>> stuffiown;

	int GetType() const
	{
		return TypeID;
	}

};


class mytarget : public ACC::component
{

public:

	static const int TypeID = 24;

	std::shared_ptr<ACC::entity> target;

	int GetType() const
	{
		return TypeID;
	}

};

class trackingarrowscomp : public ACC::component
{

public:

	static const int TypeID = 25;

	int GetType() const { return TypeID; };

	std::shared_ptr<transposecomponent> targetbox;
	std::shared_ptr<ACC::entity> player;
	glm::vec3 offset;

};

class soundComponet : public ACC::component
{

public:

	static const int TypeID = 26;
	int GetType() const { return TypeID; };

	glm::vec3 position;
	sound mysound;
	enum soundtype { repeate, onetime};
	soundtype mytype;

};
