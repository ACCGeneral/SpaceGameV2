#pragma once

#include "shadersettings.h"

class uisettings : public shadersettings
{
private:
	bool target;
	bool team;
	glm::vec3 scale;
		
public:
	uisettings()
	{
		setmytype("uisettings");
		target = false;
	}
	void settarget(bool tar)
	{
		target = tar;
	}
	void setteam(bool tea)
	{
		team = tea;
	}
	void setscale(glm::vec3 sc)
	{
		scale = sc;
	}
	void setupshader(GLuint program)
	{
		glUniform1i(glGetUniformLocation(program, "team"), team);
		glUniform1i(glGetUniformLocation(program, "target"), target);
		glUniform3f(glGetUniformLocation(program, "scale"), scale.x, scale.y, scale.z);
	}

};