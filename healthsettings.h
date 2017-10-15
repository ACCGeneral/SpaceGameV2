#pragma once

#include "shadersettings.h"

class healthsettings : public shadersettings
{
private:
	glm::vec3 scale;
	float normhealth;

public:

	healthsettings()
	{
		setmytype("healthsettings");
	}
	void setscale(glm::vec3 sc)
	{
		scale = sc;
	}
	void setnormhealth(float NH)
	{
		normhealth = NH;
	}
	void setupshader(GLuint program)
	{
		glUniform3f(glGetUniformLocation(program, "scale"), scale.x, scale.y, scale.z);
		glUniform1f(glGetUniformLocation(program, "myhealth"), normhealth);
	}

};