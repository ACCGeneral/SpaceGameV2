#include "dirlight.h"
#include <string>

void dirlight::shaderlinks(GLuint program, std::string index) //link the light data to the shader program
{

	glUniform3f(glGetUniformLocation(program, ("myDirLights[" + index + "]." + diratt).c_str()), direction.x, direction.y, direction.z);
	glUniform3f(glGetUniformLocation(program, ("myDirLights[" + index + "]." + ambatt).c_str()), Ambient.x, Ambient.y, Ambient.z);
	glUniform3f(glGetUniformLocation(program, ("myDirLights[" + index + "]." + lightatt).c_str()), lightcolour.x, lightcolour.y, lightcolour.z);

}
