#pragma once
#include <memory>
#include <string>

class world;

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