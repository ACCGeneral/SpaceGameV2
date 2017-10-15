#pragma once

#include "shadersettings.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp> 
#include <gtx/matrix_interpolation.hpp>

class orthographicsettings : public shadersettings
{
private:
	glm::mat4 orthoprojection;
	glm::vec2 translation;

public:

	orthographicsettings()
	{
		setmytype("orthographicsettings");
	}
	void setproject(glm::mat4 &project)
	{
		orthoprojection = project;
	}
	void settranslation(glm::vec2 trans)
	{
		translation = trans;
	}
	void setupshader(GLuint program)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "orthoprojection"), 1, GL_FALSE, glm::value_ptr(orthoprojection));
		glUniform2f(glGetUniformLocation(program, "translation"), translation.x, translation.y);
	}

};