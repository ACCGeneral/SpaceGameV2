#pragma once
#include <memory>
#include <string>

class world;
class teamunits;
class transposecomponent;
class directioncomponent;
class collisioncomp;
class physics;
class collisioncomp;
class fighterguns;
class AIcomp;
class fighterguns;

namespace ACC
{
	class entity;
}

class action
{
private:

protected:
	float time;
	std::shared_ptr<ACC::entity> me;
	std::string mytype;

public:

	void addme(std::shared_ptr<ACC::entity> addme);
	void settime(float t)
	{
		time = t;
	}
	float returntime()
	{
		return time;
	}
	std::string returntype()
	{
		return mytype;
	}


	virtual void start() {}
	virtual void run(float dt, std::shared_ptr<world> myworld) {}
	virtual void end() {}

};