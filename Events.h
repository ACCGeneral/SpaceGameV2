#pragma once

#include <glm.hpp>
#include "particleExplotionData.h"
#include "sound.h"


enum eventtype {healthdamage, explotion, Sound, dead_collision};

struct Event
{
	eventtype mytype;

	eventtype myeventtype()
	{
		return mytype;
	}

};


struct healthevenet : public Event
{
	healthevenet()
	{
		mytype = healthdamage;
		takingdamageID = -1;
		damagedealerID = -1;
	}

	int takingdamageID;
	int damagedealerID;
	glm::vec3 position;
	
};

struct explostionEvent : public Event
{
	explostionEvent()
	{
		mytype = explotion;
	}

	particleExplotionData explotionData;
	int ID;
};

struct soundEvent : public Event
{
	soundEvent()
	{
		mytype = explotion;
	}

	glm::vec3 postion;
	sound playsound;

};

struct DeadCol : public Event
{
	DeadCol()
	{
		mytype = dead_collision;
		ID = -1;
	}

	int ID;
};
