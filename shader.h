#pragma once

#include <glm.hpp>
#include <string>
#include <fstream>
#include <vector>
#include "common.h"

//the shader class that sets up the shaders into our project

class shader
{

private:

	GLuint program;
	
public:

	shader();
	bool loadshader(std::string shaderlocation, GLenum type);
	void useProgram();
	void stopProgram();
	bool linkcheck();
	void linkshader();
	GLuint returnprogram();
	


};

//code is posted on forum http://www.gamedev.net/topic/681733-opengl-ecs-performance-problem/#entry5308412 (i am lexington)