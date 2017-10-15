#pragma once

#include <glm.hpp>
#include <memory>
#include <gtc/matrix_transform.hpp> 
#include <gtx/matrix_interpolation.hpp>
#include <gtc/type_ptr.hpp>

class transposecomponent;

//our main camera that will be a must include for each scene

class maincamera
{
private:

	glm::mat4 Projection;
	glm::vec3 position;
	glm::vec3 direction;
	glm::mat4 View;
	std::shared_ptr<transposecomponent> playertrans;

public:

	maincamera();
	void setProjection(glm::mat4 &proj);
	void setView();
	void setView(glm::mat4 &setview);
	void setposition(glm::vec3 campos);
	void setdirection(glm::vec3 dir);

	glm::mat4 getproj()
	{
		return Projection;
	}
	glm::mat4 getview()
	{
		return View;
	}
	glm::vec3 getpos()
	{
		return position;
	}
	glm::vec3 getdir()
	{
		return direction;
	}

	void getplayerpos(std::shared_ptr<transposecomponent> ptran)
	{
		playertrans = ptran;
	
	}

};